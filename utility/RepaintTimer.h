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
  ~RepaintTimer() override
  {
    stopRepaintTimer();
    Timer::~Timer();
  }

public:
  //============================================================================
  void startRepaintTimer() noexcept
  {
    if (this->isTimerRunning())
      return;

    this->currentFps = fps;
    startTimerHz(fps);
  }
  //============================================================================
  void stopRepaintTimer() noexcept { stopTimer(); }
  //============================================================================
  virtual void repaintTimerCallback() noexcept = 0;
  //============================================================================
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
