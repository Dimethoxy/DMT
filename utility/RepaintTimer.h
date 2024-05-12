#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace utility {
class RepaintTimer : juce::Timer
{
  const int& fps = Settings::fps;

public:
  //============================================================================
  void startRepaintTimer() noexcept
  {
    this->currentFps = fps;
    startTimerHz(fps);
  }
  //============================================================================
  void stopRepaintTimer() noexcept { stopTimer(); }
  //============================================================================
  virtual void repaintTimerCallback() = 0;

private:
  //===========================================================================
  void timerCallback() override
  {
    repaintTimerCallback();
    if (fps != currentFps) {
      currentFps = fps;
      stopRepaintTimer();
      startRepaintTimer();
    }
  }
  int currentFps;
};
} // namespace utility
} // namespace dmt
