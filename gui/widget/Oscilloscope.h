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
  using ReadWriteLock = juce::ReadWriteLock;

public:
  //============================================================================
  Oscilloscope(const int channel)
    : Thread(String("Oscilloscope" + channel))
    , channel(channel)
  {
    startThread();
  }
  //============================================================================
  ~Oscilloscope() override { stopThread(1000); }
  //============================================================================
  void run() override
  {
    Graphics g(image);
    while (!threadShouldExit()) {
      wait(1000);
      const ScopedWriteLock writeLock(imageLock);
      g.setColour(Colours::white);
      g.fillRect(0, 0, image.getHeight(), image.getWidth());
    }
  }
  //============================================================================
  juce::Image getImage() const
  {
    const ScopedReadLock readLock(imageLock);
    return image;
  }
  //============================================================================
  void setSize(const int width, const int height)
  {
    const ScopedWriteLock writeLock(imageLock);
    image = Image(PixelFormat::ARGB, width, height, true);
  }
  //============================================================================
private:
  const int channel;
  Image image = Image(PixelFormat::ARGB, 100, 100, true);
  ReadWriteLock imageLock;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt