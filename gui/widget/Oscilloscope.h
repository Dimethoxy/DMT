#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class Oscilloscope : public juce::Thread
{
  using Image = juce::Image;
  using PixelFormat = Image::PixelFormat;

public:
  //==============================================================================
  Oscilloscope(int channel)
    : juce::Thread(juce::String("Oscilloscope" + channel))
  {
  }
  //==============================================================================
  void run() override
  {
    const ScopedLock myScopedLock(objectLock);
    while (!threadShouldExit()) {
      wait(1000);
      Image image(PixelFormat::ARGB, 100, 100, true);
      Graphics g(image);
      g.setColour(juce::Colours::white);
      g.fillRect(0, 0, 100, 100);
    }
  }

private:
  //==============================================================================
  CriticalSection objectLock;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
};
} // namespace widget
} // namespace gui
} // namespace dmt