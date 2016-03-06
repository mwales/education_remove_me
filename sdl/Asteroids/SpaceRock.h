#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "GraphicEntity.h"
#include "GameMath.h"

class AnimationDriver;
class TiledImage;
class GameEntity;
class ImageInfo;

class SpaceRock : public GraphicEntity
{
public:
   SpaceRock(XYPair mapBounds, SDL_Renderer* r);

   virtual ~SpaceRock();

   void SetRandomLocation(XYPair shipPos);

   void Explode(std::vector<GameEntity*>* deletionList,
                std::vector<GameEntity*>* additionList);

   virtual void Update();

   virtual void Draw() override;

   void DisplayCollisionArea(bool display);

protected:

   static bool theRunOnce;

   ImageInfo* theRockImage;

   TiledImage* theExplosionImage;

   static std::vector<std::string> theRockImageFilename;

   static std::vector<std::string> theExplosionImageFilenames;

   AnimationDriver* theAnimator;

   std::vector<GameEntity*>* theDeletionList;

   bool theDisplayCollisionArea;
};
