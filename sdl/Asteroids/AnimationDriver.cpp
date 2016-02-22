#include <math.h>
#include "AnimationDriver.h"
#include "TiledImage.h"
#include "Logger.h"

AnimationDriver::AnimationDriver(TiledImage* image, bool repeating, int startFrame, int endFrame):
   _image(image),
   _updatesPerFrame(1),
   _loopForever(repeating),
   _deleteAfterwards(false),
   _startFrame(startFrame),
   _currentUpdateCall(0)
{
   if ( (startFrame < 0) || (startFrame >= image->GetNumberOfFrames()) )
   {
      LOG_WARNING() << "Animation driver given an invalid start frame of" << startFrame
                    << "and tile image only has" << image->GetNumberOfFrames() << "frames";
      _startFrame = 0;
   }

   if ( (endFrame < 0) || (endFrame >= image->GetNumberOfFrames()) )
   {
      if (endFrame != -1)
      {
         // An invalid frame was passed to us for the end frame
         LOG_WARNING() << "Animation driver given an invalid end frame of" << startFrame
                       << "and tile image only has" << image->GetNumberOfFrames() << "frames";
      }

      _stopFrame = image->GetNumberOfFrames() - 1;
   }

   LOG_DEBUG() << "Animation driver will animate from frame" << _startFrame << "to" << _stopFrame;
   _currentFrame = _startFrame;
}

void AnimationDriver::SetDeleteAfterAnimation(bool deleteAfterwards)
{
   _deleteAfterwards = deleteAfterwards;
}

void AnimationDriver::SetInfiniteLoop(bool loopForever)
{
   _loopForever = loopForever;
}

bool AnimationDriver::StepAnimation()
{
   _currentUpdateCall++;
   if (_currentUpdateCall >= _updatesPerFrame)
   {
      _currentUpdateCall = 0;

      if (_currentFrame == _stopFrame)
      {
         // We are at the end of the animation
         if (_loopForever)
         {
            _currentFrame = _startFrame;
            return false;
         }

         LOG_DEBUG() << "Animation over!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
         _image = nullptr;

         // Need to add a list of AnimationDrivers to delete as well
         return true;
      }
      else
      {
         // Next frame
         _currentFrame++;
         _image->SetFrameNumber(_currentFrame);
      }
   }

   return false;
}

void AnimationDriver::SetAnimationDuration(float seconds, int updateRateHz)
{
   // Time it would take to run the animation at it's fasetest rate, which is 1 update/frame
   float minAnimationTime = (_stopFrame - _startFrame + 1) / (float) updateRateHz;

   float idealUpdatesPerAnimation = seconds / minAnimationTime;

   float updatesPerFrameLess = floor(idealUpdatesPerAnimation);
   float updatesPerFrameMore = updatesPerFrameLess + 1.0;

   if ( (idealUpdatesPerAnimation - updatesPerFrameLess) < (updatesPerFrameMore - idealUpdatesPerAnimation))
   {
      _updatesPerFrame = (int) (updatesPerFrameLess + 0.1);
   }
   else
   {
      _updatesPerFrame = (int) (updatesPerFrameMore + 0.1);
   }

   LOG_DEBUG() << "Setting updates per frame to" << _updatesPerFrame;
}

void AnimationDriver::SetUpdatesPerFrame(int upf)
{
   if (upf <= 0)
   {
      LOG_FATAL() << "Can't set updates per frame on animation to zero or less:" << upf;
      return;
   }

   _updatesPerFrame = upf;
}
