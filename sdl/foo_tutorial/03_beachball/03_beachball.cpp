#include <stdio.h>
#include <math.h>     // sin, cos
#include <time.h>
#include <stdlib.h>   // srand, rand

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAMERATE = 60;

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

void UpdatePosition(XYPair* position, XYPair* velocity, int const & maxX, int const & maxY)
{
   *position = AddVector(*position, *velocity);

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

   SDL_Window* window = NULL;
   SDL_Surface* screenSurface = NULL;


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

   screenSurface = SDL_GetWindowSurface(window);

   SDL_Surface* ball = IMG_Load("bb.png");
   SDL_Surface* background = IMG_Load("bg.jpg");

   if (ball == NULL)
   {
      printf("Error loading the ball image: %s\n", SDL_GetError());
      return -1;
   }

   printf("Image loaded OK\n");

   bool quit = false;
   while (!quit)
   {

      Uint32 startTick = SDL_GetTicks();

      if (SDL_MUSTLOCK(screenSurface))
      {
         SDL_UnlockSurface(screenSurface);
         printf("Surface unlocked successfully\n");
      }

      SDL_BlitScaled(background, NULL, screenSurface, NULL);

      SDL_Rect centerOfScreen = { (int) pos.x, (int) pos.y, 100, 100};
      SDL_BlitScaled(ball, NULL, screenSurface, &centerOfScreen);


      if (SDL_MUSTLOCK(screenSurface))
      {
         SDL_LockSurface(screenSurface);
         printf("Surface locked successfully\n");
      }

      UpdatePosition(&pos, &vel, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100);

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

      SDL_UpdateWindowSurface(window);
   }

   SDL_DestroyWindow(window);

   SDL_FreeSurface(ball);
   SDL_FreeSurface(background);

   SDL_Quit();

   return 0;
}

