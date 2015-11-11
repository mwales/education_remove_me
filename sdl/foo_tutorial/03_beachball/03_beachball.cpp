#include <stdio.h>
#include <math.h>     // sin, cos
#include <time.h>
#include <stdlib.h>   // srand, rand

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAMERATE = 60;
const double ROTATION_PER_FRAME = 360 * 2 / (double) FRAMERATE;

struct XYPair
{
   float x;
   float y;
};

XYPair GetUnitVector(float const & angleRadians)
{
   XYPair retVal;
   retVal.x = cosf(angleRadians);
   retVal.y = sinf(angleRadians);
   return retVal;
}

XYPair ScaleVector(XYPair base, float const & scalar)
{
   base.x *= scalar;
   base.y *= scalar;
   return base;
}

XYPair AddVector(XYPair const & a, XYPair const & b)
{
   XYPair retVal;
   retVal.x = a.x + b.x;
   retVal.y = a.y + b.y;
   return retVal;
}

void UpdatePosition(XYPair* position, XYPair* velocity, double* angle, int const & maxX, int const & maxY)
{
   *position = AddVector(*position, *velocity);
   *angle += ROTATION_PER_FRAME;
   if (*angle > 360.0)
   {
      *angle -= 360.0;
   }

   // Check x bounds
   if (position->x < 0)
   {
      printf("Reflected off of the left fence\n");
      position->x *= -1;
      velocity->x *= -1;
   }

   if (position->x > maxX)
   {
      printf("Reflected off of the left fence\n");
      position->x = maxX - (position->x - maxX);
      velocity->x *= -1;
   }

   if (position->y < 0)
   {
      printf("Reflected off the top fence\n");
      position->y *= -1;
      velocity->y *= -1;
   }

   if (position->y > maxY)
   {
      printf("Reflected off of the bottom fence\n");
      position->y = maxY - (position->y - maxY);
      velocity->y *= -1;
   }
}

SDL_Texture* LoadTexture(char const * const path, SDL_Renderer* r)
{
   // Load images into textures
   SDL_Surface* imageSurface = IMG_Load(path);

   if (imageSurface == NULL)
   {
      printf("Error loading the %s image: %s\n", path, IMG_GetError());
      return NULL;
   }

   SDL_Texture* retText = SDL_CreateTextureFromSurface(r, imageSurface);
   if (retText == NULL)
   {
      printf("Error converting the surface (%s) to a texture:  %s\n", path, SDL_GetError());
      return NULL;
   }

   SDL_FreeSurface(imageSurface);

   return retText;
}

int main(int argc, char* argv[])
{
   // Initiate random numbers
   srand(time(NULL));

   int angleDeg = rand() % 360;
   float angleRad = (float) angleDeg * 2.0 * 3.14159 / 360.0;
   printf("Initial angle = %f\n", angleRad);

   XYPair pos = { (SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT - 100) / 2 };
   XYPair vel = GetUnitVector(angleRad);
   vel = ScaleVector(vel, 200.0 / (float) FRAMERATE);
   double angle = 0;


   SDL_Window* window = NULL;
   SDL_Renderer* renderer = NULL;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      printf("SDL initialization error: %s\n", SDL_GetError());
      return -1;
   }

   SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
   printf("Main\n");

   window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
   if (window == NULL)
   {
      printf("Error creating window: %s\n", SDL_GetError());
      return -1;
   }

   renderer = SDL_CreateRenderer(window, -1, 0);
   if (renderer == NULL)
   {
      printf("Failed to create the renderer: %s\n", SDL_GetError());
      return -1;
   }

   SDL_SetRenderDrawColor(renderer, 255,255,255,255);

   // Load textures
   IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
   SDL_Texture* ball = LoadTexture("bb.png", renderer);
   SDL_Texture* bg = LoadTexture("bg.jpg", renderer);
   IMG_Quit();

   if ( (ball == NULL) || (bg == NULL) )
   {
      printf("Error loading the textures\n");
      return -1;
   }

   printf("Images loaded OK\n");

   bool quit = false;
   while (!quit)
   {

      Uint32 startTick = SDL_GetTicks();

      SDL_Rect ballPos;
      ballPos.h = 100;
      ballPos.w = 100;
      ballPos.x = pos.x;
      ballPos.y = pos.y;

      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, bg, NULL, NULL);
      SDL_RenderCopyEx(renderer, ball, NULL, &ballPos, angle, NULL, SDL_FLIP_NONE);
      SDL_RenderPresent(renderer);

      UpdatePosition(&pos, &vel, &angle, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100);

      Uint32 stopTick = SDL_GetTicks();
      Uint32 processingTime = (stopTick - startTick);

      if ( (stopTick - startTick) < (1000 / FRAMERATE) )
      {
           printf("Delaying %u ticks\n", 1000 / FRAMERATE - processingTime);
           SDL_Delay(1000 / FRAMERATE - processingTime);
      }

      SDL_Event e;
      while(SDL_PollEvent(&e) != 0)
      {
         if (e.type == SDL_QUIT)

         {
            printf("User quitting\n");
            quit = true;
         }
      }

   }


   SDL_DestroyTexture(ball);
   SDL_DestroyTexture(bg);

   SDL_DestroyRenderer(renderer);

   SDL_DestroyWindow(window);

   SDL_Quit();

   return 0;
}

