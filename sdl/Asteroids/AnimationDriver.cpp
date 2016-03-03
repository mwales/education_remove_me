#include <math.h>
#include "AnimationDriver.h"
#include "TiledImage.h"
#include "Logger.h"

AnimationDriver::AnimationDriver(TiledImage* image, bool repeating, int startFrame, int endFrame):
   theImage(image),
   theUpdatePerFrame(1),
   theLoopForever(repeating),
   theDeleteAfterwordsFlag(false),
   theStartFrame(startFrame),
   theCurrentUpdateCall(0)
{
   if ( (startFrame < 0) || (startFrame >= image->GetNumberOfFrames()) )
   {
      LOG_WARNING() << "Animation driver given an invalid start frame of" << startFrame
                    << "and tile image only has" << image->GetNumberOfFrames() << "frames";
      theStartFrame = 0;
   }

   if ( (endFrame < 0) || (endFrame >= image->GetNumberOfFrames()) )
   {
      if (endFrame != -1)
      {
         // An invalid frame was passed to us for the end frame
         LOG_WARNING() << "Animation driver given an invalid end frame of" << startFrame
                       << "and tile image only has" << image->GetNumberOfFrames() << "frames";
      }

      theStopFrame = image->GetNumberOfFrames() - 1;
   }

   LOG_DEBUG() << "Animation driver will animate from frame" << theStartFrame << "to" << theStopFrame;
   theCurrentFrame = theStartFrame;
}

void AnimationDriver::SetDeleteAfterAnimation(bool deleteAfterwards)
{
   theDeleteAfterwordsFlag = deleteAfterwards;
}

void AnimationDriver::SetInfiniteLoop(bool loopForever)
{
   theLoopForever = loopForever;
}

bool AnimationDriver::StepAnimation()
{
   theCurrentUpdateCall++;
   if (theCurrentUpdateCall >= theUpdatePerFrame)
   {
      theCurrentUpdateCall = 0;

      if (theCurrentFrame == theStopFrame)
      {
         // We are at the end of the animation
         if (theLoopForever)
         {
            theCurrentFrame = theStartFrame;
            return false;
         }

         theImage = nullptr;

         // Need to add a list of AnimationDrivers to delete as well
         return true;
      }
      else
      {
         // Next frame
         theCurrentFrame++;
         theImage->SetFrameNumber(theCurrentFrame);
      }
   }

   return false;
}

void AnimationDriver::SetAnimationDuration(float seconds, int updateRateHz)
{
   // Time it would take to run the animation at it's fasetest rate, which is 1 update/frame
   float minAnimationTime = (theStopFrame - theStartFrame + 1) / (float) updateRateHz;

   float idealUpdatesPerAnimation = seconds / minAnimationTime;

   float updatesPerFrameLess = floor(idealUpdatesPerAnimation);
   float updatesPerFrameMore = updatesPerFrameLess + 1.0;

   if ( (idealUpdatesPerAnimation - updatesPerFrameLess) < (updatesPerFrameMore - idealUpdatesPerAnimation))
   {
      theUpdatePerFrame = (int) (updatesPerFrameLess + 0.1);
   }
   else
   {
      theUpdatePerFrame = (int) (updatesPerFrameMore + 0.1);
   }

   LOG_DEBUG() << "Setting updates per frame to" << theUpdatePerFrame;
}

void AnimationDriver::SetUpdatesPerFrame(int upf)
{
   if (upf <= 0)
   {
      LOG_FATAL() << "Can't set updates per frame on animation to zero or less:" << upf;
      return;
   }

   theUpdatePerFrame = upf;
}
