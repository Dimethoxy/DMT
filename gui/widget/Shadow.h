//==============================================================================
#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <melatonin_perfetto/melatonin_perfetto.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class Shadow : public juce::Thread
{
  using Thread = juce::Thread;
  using PixelFormat = juce::Image::PixelFormat;
  using ReadWriteLock = juce::ReadWriteLock;

public:
  Shadow(const juce::String name,
         const juce::Colour& colour,
         const float& radius,
         const bool inner)
    : Thread(name + "Thread")
    , colour(colour)
    , radius(radius)
    , inner(inner)
  {
    startThread();
  }
  //============================================================================
  void paint(juce::Graphics& g) {}
  //============================================================================
  juce::Image getImage() const
  {
    const ScopedReadLock readLock(imageLock);
    return image.createCopy();
  }
  //============================================================================
  void setBounds(juce::Rectangle<int> newBounds)
  {
    resizeImage(newBounds.getWidth(), newBounds.getHeight());
    bounds = newBounds;
  }
  //============================================================================
  juce::Rectangle<int> getBounds() const { return bounds; }

protected:
  //============================================================================
  void run() override
  {
    while (!threadShouldExit()) {
      wait(10000);
      const ScopedWriteLock writeLock(imageLock);
      render();
    }
  }
  //==============================================================================
  void render()
  {
    TRACER("Shadow::render");

    if (path.isEmpty())
      return;

    juce::Graphics imageGraphics(image);
    imageGraphics.setColour(colour);

    if (inner) {
      drawInnerForPath(imageGraphics, path);
    } else {
      drawOuterForPath(imageGraphics, path);
    }
  }
  //==============================================================================
  void resizeImage(const int width, const int height)
  {
    TRACER("Shadow::resizeImage");
    // Resize the image
    const ScopedWriteLock writeLock(imageLock);
    image = Image(PixelFormat::ARGB, width + 10, height, true);
  }
  //============================================================================
  void drawInnerForPath(juce::Graphics& g, juce::Path target)
  {
    TRACER("Shadow::drawInnerForPath");
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(target);
    juce::DropShadow ds(colour, (int)radius, offset);
    ds.drawForPath(g, shadowPath);
  }
  //============================================================================
  void drawOuterForPath(juce::Graphics& g, juce::Path target)
  {
    TRACER("Shadow::drawOuterForPath");
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(shadowPath);
    juce::DropShadow ds(colour, (int)radius, offset);
    ds.drawForPath(g, target);
  }

private:
  //============================================================================
  const juce::Colour& colour;
  juce::Point<int> offset = { 0, 0 };
  const float& radius;
  const bool inner;
  juce::Path path;
  //==============================================================================
  juce::Rectangle<int> bounds = juce::Rectangle<int>(0, 0, 1, 1);
  Image image = Image(PixelFormat::ARGB, 1, 1, true);
  ReadWriteLock imageLock;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Shadow)
};

//==============================================================================
} // namespace widget
} // namespace gui
} // namespace dmt