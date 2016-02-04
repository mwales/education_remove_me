#include "SpaceRock.h"
#include "TiledImage.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "AnimationDriver.h"

int ROCK_MAX_SPEED = 30;
int ROCK_MAX_ROT_SPEED = 90;

// Static variable initializers
bool SpaceRock::_runOnce = true;
std::vector<std::string> SpaceRock::_rockImageFilenames;
std::vector<std::string> SpaceRock::_explosionImageFilenames;

SpaceRock::SpaceRock(XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   _animator(NULL),
   _deletionList(NULL)
{
   if (_runOnce)
   {
      _rockImageFilenames.push_back("assets/asteroid_blend.png");
      _rockImageFilenames.push_back("assets/asteroid_blue.png");
      _rockImageFilenames.push_back("assets/asteroid_brown.png");

      _explosionImageFilenames.push_back("assets/explosion_blue.png");
      _explosionImageFilenames.push_back("assets/explosion_blue2.png");
      _explosionImageFilenames.push_back("assets/explosion_orange.png");

      _runOnce = false;
   }

   std::string filenameRock = _rockImageFilenames[rand() % _rockImageFilenames.size()];
   std::string filenameExplosion = _explosionImageFilenames[rand() % _explosionImageFilenames.size()];

   _rockImage = new ImageInfo(filenameRock.c_str(), r);
   _explosionImage = new TiledImage(filenameExplosion.c_str(), r, 24, 1, 0, TiledImage::CALCULATE_SINGLE_TILE_DIMENSIONS);
   SetImageInfo(_rockImage);

   _translationalFrictionScalar = 0;
   _rotationalFrictionScalar = 0;
}

SpaceRock::~SpaceRock()
{
   LOG_DEBUG() << "SpaceRock destruction (" << (unsigned long) this << ")";

   delete _rockImage;
   delete _explosionImage;

   // Image will be set to one of the 2 objects above, don't delete it in parent
   _image = NULL;

   if (_animator)
   {
      delete _animator;
      _animator = NULL;
   }
}

void SpaceRock::Explode(std::vector<GameEntity*>* deletionList,
                        std::vector<GameEntity*>* additionList)
{
   LOG_DEBUG() << "Rock exploding";
   SetImageInfo(_explosionImage);
   _animator = new AnimationDriver(_explosionImage, false);
   _animator->SetAnimationDuration(10, _updateRate);

   // Remember the reference to the deletion list
   _deletionList = deletionList;
}


void SpaceRock::SetRandomLocation(XYPair shipPos)
{
   int loopGuard = 5;
   do
   {
      _position[0] = rand() % (int) _mapBounds[0];
      _position[1] = rand() % (int) _mapBounds[1];
      loopGuard--;
   } while (loopGuard && (GameMath::Distance(shipPos, _position) < 50) );

   if (!loopGuard)
   {
      LOG_WARNING() << "Space rock location used maximum number of iterations to find position";
   }

   _angle = rand() % 360;

   _velocity = GameMath::GetUnitVector(_angle);
   _velocity *= rand() % ROCK_MAX_SPEED;
   _rotVelocity = (rand() % (ROCK_MAX_ROT_SPEED * 2)) - ROCK_MAX_ROT_SPEED;


}

void SpaceRock::Update()
{
   GraphicEntity::Update();

   if (_animator)
   {
      // Step the iteration
      if (_animator->StepAnimation())
      {
         // Returns true when animation finishes
         delete _animator;
         _animator = NULL;

         LOG_DEBUG() << "Animation finished for (" << (unsigned long) this << ")";
         _deletionList->push_back(this);
      }
   }
}
