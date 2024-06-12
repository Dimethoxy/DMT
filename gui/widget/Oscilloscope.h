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
  using Graphics = juce::Graphics;
  using String = juce::String;
  using Thread = juce::Thread;
  using PixelFormat = juce::Image::PixelFormat;

public:
  //============================================================================
  Oscilloscope(const int channel)
    : Thread(String("Oscilloscope" + channel))
    , channel(channel)
  {
    startThread();
  }
  //============================================================================
  ~Oscilloscope() override
  {
    stopThread(1000);
    notify();
  }
  //============================================================================
  void run() override
  {
    Graphics g(image);
    while (!threadShouldExit()) {
      wait(1000);
      g.setColour(Colours::white);
      g.fillRect(0, 0, 100, 100);
    }
  }
  //============================================================================
private:
  const int channel;
  juce::Atomic<int> width = 1;
  juce::Atomic<int> height = 1;
  //============================================================================
  Image image = Image(PixelFormat::ARGB, 100, 100, true);
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt