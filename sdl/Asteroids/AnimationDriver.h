#pragma once

#include <vector>

class TiledImage;
class GraphicEntity;

/**
 * Keeps tracks of and updates animation frame indexes
 */
class AnimationDriver
{
public:
   AnimationDriver(TiledImage* image, bool repeating, int startFrame = -1, int endFrame = -1);

   void SetDeleteAfterAnimation(bool deleteAfterwards);

   void SetInfiniteLoop(bool loopForever);

   // Returns true if animation completes
   bool StepAnimation();

   void SetAnimationDuration(float seconds, int updateRateHz);

   void SetUpdatesPerFrame(int upf);

protected:

   TiledImage* theImage;

   int theUpdatePerFrame;

   bool theLoopForever;

   bool theDeleteAfterwordsFlag;

   int theStartFrame;

   int theStopFrame;

   int theCurrentFrame;

   int theCurrentUpdateCall;
};
