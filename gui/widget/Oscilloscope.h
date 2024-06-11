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
public:
  //==============================================================================
  Oscilloscope(int channel)
    : juce::Thread(juce::String("Oscilloscope" + channel))
  {
  }
  //==============================================================================
  void run() override
  {
    juce::Image image(juce::Image::PixelFormat::ARGB, 100, 100, true);
    juce::Graphics g(image);
    while (!threadShouldExit()) {
      wait(1000);
      g.setColour(juce::Colours::white);
      g.fillRect(0, 0, 100, 100);
    }
  }
};
} // namespace widget
} // namespace gui
} // namespace dmt