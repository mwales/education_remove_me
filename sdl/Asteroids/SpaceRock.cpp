#include "SpaceRock.h"
#include "TiledImage.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "AnimationDriver.h"

int ROCK_MAX_SPEED = 30;
int ROCK_MAX_ROT_SPEED = 90;

bool SpaceRock::_loadImagesOnce = true;
std::vector<ImageInfo*> SpaceRock::_rockImages;
std::vector<TiledImage*> SpaceRock::_explosionImages;

SpaceRock::SpaceRock(XYPair mapBounds, SDL_Renderer* r):
   GraphicEntity(mapBounds),
   _animator(NULL),
   _deletionList(NULL)
{
   if (_loadImagesOnce)
   {
      _rockImages.push_back(new ImageInfo("assets/asteroid_blend.png",r));
      _rockImages.push_back(new ImageInfo("assets/asteroid_blue.png",r));
      _rockImages.push_back(new ImageInfo("assets/asteroid_brown.png",r));

      _explosionImages.push_back(new TiledImage("assets/explosion_blue.png", r,24,1, 0,
                                                TiledImage::CALCULATE_SINGLE_TILE_DIMENSIONS));
      _explosionImages.push_back(new TiledImage("assets/explosion_blue2.png", r,24,1, 0,
                                                TiledImage::CALCULATE_SINGLE_TILE_DIMENSIONS));
      _explosionImages.push_back(new TiledImage("assets/explosion_orange.png", r,24,1, 0,
                                                TiledImage::CALCULATE_SINGLE_TILE_DIMENSIONS));

      _loadImagesOnce = false;
   }

   SetImageInfo(_rockImages[rand() % _rockImages.size()]);

   _translationalFrictionScalar = 0;
   _rotationalFrictionScalar = 0;
}

SpaceRock::~SpaceRock()
{
   LOG_DEBUG() << "SpaceRock destruction (" << (unsigned long) this << ")";

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
   TiledImage* explosionImg = _explosionImages[rand() % _explosionImages.size()];
   SetImageInfo(explosionImg);
   _animator = new AnimationDriver(explosionImg, false);
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
   LOG_DEBUG() << "Updating spacerock (" << (unsigned long) this << ")";

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
