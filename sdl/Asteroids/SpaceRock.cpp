#include "SpaceRock.h"
#include "TiledImage.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "AnimationDriver.h"
#include "ILifetimeManager.h"
#include "ShootingSceneConstants.h"

int ROCK_MAX_SPEED = 30;
int ROCK_MAX_ROT_SPEED = 90;

// Static variable initializers
bool SpaceRock::theRunOnce = true;
std::vector<std::string> SpaceRock::theRockImageFilename;
std::vector<std::string> SpaceRock::theExplosionImageFilenames;

SpaceRock::SpaceRock(XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   theAnimator(nullptr),
   theLifetimeMgr(nullptr),
   theDisplayCollisionArea(false)
{
   if (theRunOnce)
   {
      theRockImageFilename.push_back("assets/asteroid_blend.png");
      theRockImageFilename.push_back("assets/asteroid_blue.png");
      theRockImageFilename.push_back("assets/asteroid_brown.png");

      theExplosionImageFilenames.push_back("assets/explosion_blue.png");
      theExplosionImageFilenames.push_back("assets/explosion_blue2.png");
      theExplosionImageFilenames.push_back("assets/explosion_orange.png");

      theRunOnce = false;
   }

   std::string filenameRock = theRockImageFilename[rand() % theRockImageFilename.size()];
   std::string filenameExplosion = theExplosionImageFilenames[rand() % theExplosionImageFilenames.size()];

   theRockImage = new ImageInfo(filenameRock.c_str(), r);
   theExplosionImage = new TiledImage(filenameExplosion.c_str(), r, 24, 1, 0, TiledImage::CALCULATE_SINGLE_TILE_DIMENSIONS);
   SetImageInfo(theRockImage);

   theTranslationalFrictionScalar = 0;
   theRotationalFrictionScalar = 0;

   LOG_DEBUG() << "Space Rock constructed at " << (long) this;
}

SpaceRock::~SpaceRock()
{
   LOG_DEBUG() << "SpaceRock destruction (" << (unsigned long) this << ")";

   delete theRockImage;
   delete theExplosionImage;

   // Image will be set to one of the 2 objects above, don't delete it in parent
   theImage = nullptr;

   if (theAnimator)
   {
      delete theAnimator;
      theAnimator = nullptr;
   }
}

void SpaceRock::Explode(ILifetimeManager* mgr)
{
   LOG_DEBUG() << "Rock exploding";
   SetImageInfo(theExplosionImage);
   theExplosionImage->SetSize(theRockImage->GetSize());
   theAnimator = new AnimationDriver(theExplosionImage, false);
   theAnimator->SetAnimationDuration(1, theUpdateRateHz);

   // Remember the reference to the lifetime manager
   theLifetimeMgr = mgr;
}


void SpaceRock::SetRandomLocation(XYPair shipPos)
{
   int loopGuard = 5;
   do
   {
      thePosition[0] = rand() % (int) theMapBounds[0];
      thePosition[1] = rand() % (int) theMapBounds[1];
      loopGuard--;
   } while (loopGuard && (GameMath::Distance(shipPos, thePosition) < 50) );

   if (!loopGuard)
   {
      LOG_WARNING() << "Space rock location used maximum number of iterations to find position";
   }

   theAngle = rand() % 360;

   theVelocity = GameMath::GetUnitVector(theAngle);
   theVelocity *= rand() % ROCK_MAX_SPEED;
   theRotVelocity = (rand() % (ROCK_MAX_ROT_SPEED * 2)) - ROCK_MAX_ROT_SPEED;


}

void SpaceRock::Update()
{
   GraphicEntity::Update();

   if (theAnimator)
   {
      // Step the iteration
      if (theAnimator->StepAnimation())
      {
         // Returns true when animation finishes
         delete theAnimator;
         theAnimator = nullptr;

         LOG_DEBUG() << "Animation finished for (" << (unsigned long) this << ")";
         theLifetimeMgr->DeleteEntity(this, ShootingSceneLifetimeCodes::REMOVE_CODE_BIG_ROCK);
      }
   }
}

void SpaceRock::Draw()
{
   GraphicEntity::Draw();

   if (theDisplayCollisionArea)
   {
      SDL_Renderer* r = theImage->GetRenderer();

      for(auto singleBox : GetCollisionBoxes())
      {

         SDL_RenderDrawRect(r, &singleBox);
      }
   }
}

void SpaceRock::DisplayCollisionArea(bool display)
{
   theDisplayCollisionArea = display;
}
