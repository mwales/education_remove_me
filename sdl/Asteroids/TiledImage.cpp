#include "TiledImage.h"
#include "Logger.h"

TiledImage::TiledImage(char const * filename,
           SDL_Renderer* renderer,
           int tileWidth,
           int tileHeight,
           int spacing,
           TilingMode mode):
   ImageInfo(filename, renderer),
   _frameSpacing(spacing),
   _currentFrameNumber(0)
{
   XYPair fullImageDim = _size;

   if (mode == PROVIDING_SINGLE_TILE_DIMENSIONS)
   {
      _framesWide = (fullImageDim[0] + _frameSpacing) / (tileWidth + _frameSpacing);
      _framesHigh = (fullImageDim[1] + _frameSpacing) / (tileHeight + _frameSpacing);

      int calculatedWidth = _framesWide * (tileWidth + _frameSpacing) - _frameSpacing;
      int calculatedHeight = _framesHigh * (tileHeight + _frameSpacing) - _frameSpacing;

      if ( (calculatedWidth != _size[0]) || (calculatedHeight != _size[1]) )
      {
         LOG_FATAL() << "Tileset dimensions invalid.  (" << tileWidth << "," << tileHeight << ")"
                     << "in a set" << _framesWide << "x" << _framesHigh << "with spacing"
                     << _frameSpacing << "should be (" << calculatedWidth << "," << calculatedHeight
                     << "), not" << _size;
      }
      else
      {
         _size[0] = tileWidth;
         _size[1] = tileHeight;

         LOG_DEBUG() << "Tileset has" << _framesWide * _framesHigh << "frames, tiles are" << _size;
      }
   }
   else
   {
      // mode == CALCULATE_SINGLE_TILE_DIMENSIONS
      _framesWide = tileWidth;
      _framesHigh = tileHeight;

      int singleTileWidth = (fullImageDim[0] - (_frameSpacing * (_framesWide -1) )) / _framesWide;
      int singleTileHeight = (fullImageDim[1] - (_frameSpacing * (_framesHigh -1) )) / _framesHigh;

      int calculatedWidth = _framesWide * (singleTileWidth + _frameSpacing) - _frameSpacing;
      int calculatedHeight = _framesHigh * (singleTileHeight + _frameSpacing) - _frameSpacing;

      if ( (calculatedWidth != _size[0]) || (calculatedHeight != _size[1]) )
      {
         LOG_FATAL() << "Tileset dimensions invalid.  (" << singleTileWidth << "," << singleTileHeight << ")"
                     << "in a set" << _framesWide << "x" << _framesHigh << "with spacing"
                     << _frameSpacing << "should be (" << calculatedWidth << "," << calculatedHeight
                     << "), not" << _size;
      }
      else
      {
         _size[0] = singleTileWidth;
         _size[1] = singleTileHeight;

         LOG_DEBUG() << "Tileset has" << _framesWide * _framesHigh << "frames, tiles are" << _size;
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
   return _framesWide * _framesHigh;
}

void TiledImage::SetFrameNumber(int frame)
{
   if ( (frame < 0) || (frame >= GetNumberOfFrames()) )
   {
      LOG_FATAL() << "Invalid frame (" << frame << ") specified";
   }
   else
   {
      _currentFrameNumber = frame;

      _src.w = _size[0];
      _src.h = _size[1];
      int column = _currentFrameNumber % _framesWide;
      int row = _currentFrameNumber / _framesWide;
      _src.x = column * (_size[0] + _frameSpacing);
      _src.y = row * (_size[1] + _frameSpacing);
   }
}
