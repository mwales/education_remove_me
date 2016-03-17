#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "GraphicEntity.h"
#include "GameMath.h"

class AnimationDriver;
class TiledImage;
class GameEntity;
class ImageInfo;
class ILifetimeManager;

class SpaceRock : public GraphicEntity
{
public:
   SpaceRock(XYPair mapBounds, SDL_Renderer* r);

   virtual ~SpaceRock();

   void SetRandomLocation(XYPair shipPos);

   void Explode(ILifetimeManager* mgr);

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

   ILifetimeManager* theLifetimeMgr;

   bool theDisplayCollisionArea;
};
