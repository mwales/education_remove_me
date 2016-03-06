#pragma once

#include "ImageInfo.h"

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


};

