#include "TiledImage.h"
#include "Logger.h"

TiledImage::TiledImage(char const * filename,
           SDL_Renderer* renderer,
           int tileWidth,
           int tileHeight,
           int spacing,
           TilingMode mode):
   ImageInfo(filename, renderer),
   theFrameSpacing(spacing),
   theCurrentFrameNumber(0)
{
   XYPair fullImageDim = theSize;

   if (mode == PROVIDING_SINGLE_TILE_DIMENSIONS)
   {
      theFramesWide = (fullImageDim[0] + theFrameSpacing) / (tileWidth + theFrameSpacing);
      theFramesHigh = (fullImageDim[1] + theFrameSpacing) / (tileHeight + theFrameSpacing);

      int calculatedWidth = theFramesWide * (tileWidth + theFrameSpacing) - theFrameSpacing;
      int calculatedHeight = theFramesHigh * (tileHeight + theFrameSpacing) - theFrameSpacing;

      if ( (calculatedWidth != theSize[0]) || (calculatedHeight != theSize[1]) )
      {
         LOG_FATAL() << "Tileset dimensions invalid.  (" << tileWidth << "," << tileHeight << ")"
                     << "in a set" << theFramesWide << "x" << theFramesHigh << "with spacing"
                     << theFrameSpacing << "should be (" << calculatedWidth << "," << calculatedHeight
                     << "), not" << theSize;
      }
      else
      {
         theSize[0] = tileWidth;
         theSize[1] = tileHeight;

         LOG_DEBUG() << "Tileset has" << theFramesWide * theFramesHigh << "frames, tiles are" << theSize;
      }
   }
   else
   {
      // mode == CALCULATE_SINGLE_TILE_DIMENSIONS
      theFramesWide = tileWidth;
      theFramesHigh = tileHeight;

      int singleTileWidth = (fullImageDim[0] - (theFrameSpacing * (theFramesWide -1) )) / theFramesWide;
      int singleTileHeight = (fullImageDim[1] - (theFrameSpacing * (theFramesHigh -1) )) / theFramesHigh;

      int calculatedWidth = theFramesWide * (singleTileWidth + theFrameSpacing) - theFrameSpacing;
      int calculatedHeight = theFramesHigh * (singleTileHeight + theFrameSpacing) - theFrameSpacing;

      if ( (calculatedWidth != theSize[0]) || (calculatedHeight != theSize[1]) )
      {
         LOG_FATAL() << "Tileset dimensions invalid.  (" << singleTileWidth << "," << singleTileHeight << ")"
                     << "in a set" << theFramesWide << "x" << theFramesHigh << "with spacing"
                     << theFrameSpacing << "should be (" << calculatedWidth << "," << calculatedHeight
                     << "), not" << theSize;
      }
      else
      {
         theSize[0] = singleTileWidth;
         theSize[1] = singleTileHeight;

         LOG_DEBUG() << "Tileset has" << theFramesWide * theFramesHigh << "frames, tiles are" << theSize;
      }
   }

   SetFrameNumber(0);
}

TiledImage::~TiledImage()
{
   // Intentionally blank
}

int TiledImage::GetNumberOfFrames()
{
   return theFramesWide * theFramesHigh;
}

void TiledImage::SetFrameNumber(int frame)
{
   if ( (frame < 0) || (frame >= GetNumberOfFrames()) )
   {
      LOG_FATAL() << "Invalid frame (" << frame << ") specified";
   }
   else
   {
      theCurrentFrameNumber = frame;

      theSrc.w = theSize[0];
      theSrc.h = theSize[1];
      int column = theCurrentFrameNumber % theFramesWide;
      int row = theCurrentFrameNumber / theFramesWide;
      theSrc.x = column * (theSize[0] + theFrameSpacing);
      theSrc.y = row * (theSize[1] + theFrameSpacing);
   }
}
