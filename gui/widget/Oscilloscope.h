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
  juce::Image getImage() const
  {
    const ScopedReadLock readLock(imageLock);
    return image.createCopy();
  }
  //============================================================================
  void setBounds(juce::Rectangle<int> newBounds)
  {
    resizeImage(bounds.getWidth(), bounds.getHeight());
    bounds = newBounds;
  }
  //============================================================================
  juce::Rectangle<int> getBounds() const { return bounds; }

protected:
  //============================================================================
  void run() override
  {
    TRACE_COMPONENT();
    while (!threadShouldExit()) {
      wait(10000);
      const ScopedWriteLock writeLock(imageLock);
      const Graphics g(image);
      g.fillAll(juce::Colours::coral);
    }
  }
  //============================================================================
  void resizeImage(const int width, const int height)
  {
    TRACE_COMPONENT();
    const ScopedWriteLock writeLock(imageLock);
    image = Image(PixelFormat::ARGB, width, height, true);
  }

private:
  //============================================================================
  const int channel;
  juce::Rectangle<int> bounds = juce::Rectangle<int>(0, 0, 1, 1);
  Image image = Image(PixelFormat::ARGB, 1, 1, true);
  ReadWriteLock imageLock;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscilloscope)
};
} // namespace widget
} // namespace gui
} // namespace dmt