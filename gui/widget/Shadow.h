//==============================================================================
#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class Shadow : public juce::Component
{
  using Image = juce::Image;
  using Graphics = juce::Graphics;
  using String = juce::String;
  using PixelFormat = juce::Image::PixelFormat;

public:
  Shadow(const juce::String name,
         const juce::Colour& colour,
         const float& radius,
         const bool inner)
    : colour(colour)
    , radius(radius)
    , inner(inner)
  {
  }
  //============================================================================
  void paint(juce::Graphics& g)
  {
    TRACER("Shadow::paint");
    g.drawImageAt(image, 0, 0);
  }
  //============================================================================
  void resized() override
  {
    TRACER("Shadow::resized");
    image = Image(PixelFormat::ARGB, getWidth(), getHeight(), true);
    juce::Graphics g(image);
    g.setColour(colour);
    if (inner)
      drawInnerForPath(g, path);
    else
      drawOuterForPath(g, path);
  }
  //============================================================================
  void setPath(juce::Path newPath)
  {
    TRACER("Shadow::setPath");
    path = newPath;
    resized();
  }

protected:
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
  Image image = Image(PixelFormat::ARGB, 1, 1, true);
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Shadow)
};

//==============================================================================
} // namespace widget
} // namespace gui
} // namespace dmt