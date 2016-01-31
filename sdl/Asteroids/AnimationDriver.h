#pragma once

#include <vector>

class TiledImage;
class GraphicEntity;

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

   TiledImage* _image;

   int _updatesPerFrame;

   bool _loopForever;

   bool _deleteAfterwards;

   int _startFrame;

   int _stopFrame;

   int _currentFrame;

   int _currentUpdateCall;
};
