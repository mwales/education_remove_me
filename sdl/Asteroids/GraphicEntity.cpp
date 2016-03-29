#include <math.h>

#include "GraphicEntity.h"
#include "ImageInfo.h"
#include "Logger.h"
#include "TiledImage.h"
#include "TextImage.h"
#include "GameMath.h"

const float FRICTION_SCALAR = 0.1;
const float ROT_FRICTION_SCALAR = 0.3;

GraphicEntity::GraphicEntity(XYPair mapBounds):
   theImage(nullptr),
   theFullscreen(false),
   theMapBounds(mapBounds),
   theTranslationalFrictionScalar(FRICTION_SCALAR),
   theRotationalFrictionScalar(ROT_FRICTION_SCALAR)
{
   thePosition = XYPair(0,0);
   theAngle = 0.0;

   theUpdateRateScalar = 0.0;

   theVelocity = XYPair(0.0, 0.0);

   theAcceleration = XYPair(0.0, 0.0);

   theRotVelocity = 0.0;

   theRotAcceleration = 0.0;
}

GraphicEntity::~GraphicEntity()
{
   LOG_DEBUG() << "Graphic Entity destructor (" << (unsigned long) this << ")";

   LOG_DEBUG() << "~GraphicEntity: this = " << theImage;
   if (theImage != nullptr)
   {
      delete theImage;
      theImage = nullptr;
   }
}

void GraphicEntity::Update()
{
   theVelocity += theAcceleration * theUpdateRateScalar;

   theVelocity *= (1.0 - theTranslationalFrictionScalar * theUpdateRateScalar);

   thePosition += theVelocity * theUpdateRateScalar;

   theRotVelocity += theRotAcceleration * theUpdateRateScalar;

   theRotVelocity *= (1.0 - theRotationalFrictionScalar * theUpdateRateScalar);

   AddAngle(theRotVelocity * theUpdateRateScalar);

   // Wrap position around
   thePosition = GameMath::PositionModulus(thePosition, theMapBounds);
}

void GraphicEntity::SetUpdateRate(int updateHz)
{
   theUpdateRateHz = updateHz;
   theUpdateRateScalar = 1.0 / (float) updateHz;
}

void GraphicEntity::SetVelocity(XYPair vel)
{
   theVelocity = vel;
}

XYPair GraphicEntity::GetVelocity() const
{
   return theVelocity;
}

void GraphicEntity::SetAcceleration(XYPair acc)
{
   theAcceleration = acc;
}

void GraphicEntity::SetAngularVelocity(float degPerSec)
{
   theRotVelocity = degPerSec;
}

void GraphicEntity::SetAngularAcceleration(float degPerSecSquared)
{
   theRotAcceleration = degPerSecSquared;
}

void GraphicEntity::SetTranslationalFriction(float scalar)
{
   theTranslationalFrictionScalar = scalar;
}

void GraphicEntity::SetRotationalFriction(float scalar)
{
   theRotationalFrictionScalar = scalar;
}

void GraphicEntity::Draw()
{
   if (theImage == nullptr)
   {
      LOG_WARNING() << "Drawing graphical entity with no image information";
      return;
   }

   if (theFullscreen)
   {
      theImage->DrawFullScreen();
   }
   else
   {
      theImage->Draw(thePosition);
   }
}

void GraphicEntity::SetImageInfo(char const * filename, SDL_Renderer* r)
{
   theImage = new ImageInfo(filename, r);
}

void GraphicEntity::SetTextImageInfo(std::string text, SDL_Color color, SDL_Renderer* r)
{
   theImage = new TextImage(text, color, r);
}

void GraphicEntity::SetImageInfo(ImageInfo* ii)
{
   theImage = ii;
}

void GraphicEntity::SetTiledImageInfo(char const * filename,
                                      SDL_Renderer* renderer,
                                      int width,
                                      int height,
                                      int spacing,
                                      TiledImage::TilingMode mode)
{
   theImage = new TiledImage(filename, renderer, width, height, spacing, mode);
}


void GraphicEntity::SetAngle(float degrees)
{
   theAngle = degrees;

   // Force the angle to be between 0 and 360
   if (theAngle < 0.0)
   {
      // Negative angle
      LOG_DEBUG() << "Wrapping negative angle" << theAngle << " fmod=" << fmod(theAngle, 360.0);

      theAngle *= -1;
      theAngle = fmod(theAngle, 360.0);
      theAngle = 360.0 - theAngle;

      LOG_DEBUG() << "The long way=" << theAngle ;

   }
   else if (theAngle > 360.0)
   {
      LOG_DEBUG() << "Wrapping angle around from" << theAngle << "to" << fmod(theAngle, 360.0);
      theAngle = fmod(theAngle, 360.0);
   }

   if (theImage != nullptr)
   {
      theImage->SetAngle(theAngle);
   }
}

void GraphicEntity::AddAngle(float addDegrees)
{
   SetAngle(theAngle + addDegrees);
}

void GraphicEntity::SetPosition(XYPair pos)
{
   thePosition = pos;
   theFullscreen = false;
}

XYPair GraphicEntity::GetPosition() const
{
   return thePosition;
}

std::vector<SDL_Rect> GraphicEntity::GetCollisionBoxes() const
{
   // Basic collision box is the size of the image
   XYPair rectSize = theImage->GetSize();
   XYPair topLeftPoint = thePosition - rectSize * 0.5;

   SDL_Rect basicRectangle { (int) topLeftPoint[0], (int) topLeftPoint[1], (int) rectSize[0], (int) rectSize[1] };
   std::vector<SDL_Rect> retVal;
   retVal.push_back(basicRectangle);
   return retVal;
}

void GraphicEntity::SetFullscreen(bool fullscreenEnable)
{
   theFullscreen = fullscreenEnable;
   if (theFullscreen)
   {
      thePosition[0] = 0;
      thePosition[1] = 0;
   }
}

void GraphicEntity::SetFrameNumber(int fn)
{
   theImage->SetFrameNumber(fn);
}
