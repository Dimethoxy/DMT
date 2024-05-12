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
  void repaintTimerCallback()
  {
    const auto component = dynamic_cast<juce::Component*>(this);
    if (component != nullptr)
      component->repaint();
  }

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
