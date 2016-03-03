#pragma once

#include "GameEntity.h"
#include "ICollidable.h"

#include <SDL2/SDL.h>
#include "GameMath.h"
#include "TiledImage.h"

class ImageInfo;

class GraphicEntity: public GameEntity, public ICollidable
{
public:
   GraphicEntity(XYPair mapBounds);

   virtual ~GraphicEntity();

   virtual void SetImageInfo(char const * filename, SDL_Renderer* r);

   virtual void SetTextImageInfo(std::string text, SDL_Color color, SDL_Renderer* r);

   virtual void SetImageInfo(ImageInfo* ii);

   virtual void SetTiledImageInfo(char const * filename,
                                  SDL_Renderer* renderer,
                                  int width,
                                  int height,
                                  int spacing,
                                  TiledImage::TilingMode mode);

   virtual void Update() override;

   virtual void Draw() override;

   void SetAngle(float degrees);

   void AddAngle(float addDegrees);

   void SetPosition(XYPair pos);

   void SetFullscreen(bool fullscreenEnable);

   void SetFrameNumber(int fn);

   inline ImageInfo* GetImageInfo() { return theImage; }

   virtual void SetUpdateRate(int updateHz) override;

   void SetVelocity(XYPair vel);

   void SetAcceleration(XYPair acc);

   void SetAngularVelocity(float degPerSec);

   void SetAngularAcceleration(float degPerSecSquared);

   void SetTranslationalFriction(float scalar);

   void SetRotationalFriction(float scalar);

   // ICollidable interface

   virtual XYPair GetPosition() const override;

   virtual std::vector<SDL_Rect> GetCollisionBoxes() const override;

   //virtual bool CollisionDetected(std::string collidingObject) const;

   //virtual std::string GetName() const;


protected:

   XYPair thePosition;

   ImageInfo* theImage;

   bool theFullscreen;

   float theAngle;

   XYPair theMapBounds;

   XYPair theGraphicBounds;

   float theUpdateRateScalar;

   XYPair theVelocity;

   XYPair theAcceleration;

   float theRotVelocity;

   float theRotAcceleration;

   float theTranslationalFrictionScalar;

   float theRotationalFrictionScalar;
};
