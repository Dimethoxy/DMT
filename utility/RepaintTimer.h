#pragma once
//==============================================================================
#include "utility/LibrarySettings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace utility {
class RepaintTimer : juce::Timer
{
  const int& fps = LibrarySettings::fps;

public:
  //============================================================================
  void start() noexcept
  {
    this->currentFps = fps;
    startTimerHz(fps);
  }
  //============================================================================
  void stop() noexcept { stopTimer(); }
  //===========================================================================
  void timerCallback() override
  {
    repaintTimerCallback();
    if (fps != currentFps) {
      currentFps = fps;
      stop();
      start();
    }
  }
  //===========================================================================
  virtual void repaintTimerCallback() = 0;

private:
  int currentFps;
};
} // namespace utility
} // namespace dmt
