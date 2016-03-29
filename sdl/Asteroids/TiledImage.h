#pragma once

#include "ImageInfo.h"
#include "GameMath.h"

class TiledImage : public ImageInfo
{
public:
   enum TilingMode
   {
      PROVIDING_SINGLE_TILE_DIMENSIONS,
      CALCULATE_SINGLE_TILE_DIMENSIONS
   };

   TiledImage(char const * filename,
              SDL_Renderer* renderer,
              int width,
              int height,
              int spacing,
              TilingMode mode = CALCULATE_SINGLE_TILE_DIMENSIONS);

   virtual ~TiledImage();

   virtual int GetNumberOfFrames();

   virtual void SetFrameNumber(int frame);

protected:

   int theFramesWide;

   int theFramesHigh;

   int theFrameSpacing;

   int theCurrentFrameNumber;

   /**
    * This is not the size of what is rendered, it is the size of the image frame in source.  If
    * image is scaled, we don't change this property, only the rendered image
    */
   XYPair theTileSize;


};

