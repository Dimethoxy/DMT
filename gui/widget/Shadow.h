//==============================================================================
#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
struct Shadow
{
  Shadow(const juce::Colour& colour, const float& radius)
    : colour(colour)
    , radius(radius)
  {
  }
  void drawInnerForPath(juce::Graphics& g, juce::Path target)
  {
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(target);
    juce::DropShadow ds(colour, (int)radius, offset);
    ds.drawForPath(g, shadowPath);
  }
  void drawOuterForPath(juce::Graphics& g, juce::Path target)
  {
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(shadowPath);
    juce::DropShadow ds(colour, (int)radius, offset);
    ds.drawForPath(g, target);
  }
  const juce::Colour& colour;
  juce::Point<int> offset = { 0, 0 };
  const float& radius;
};
//==============================================================================
} // namespace widget
} // namespace gui
} // namespace dmt