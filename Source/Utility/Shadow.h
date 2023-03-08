//==============================================================================
#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
struct Shadow
{
  void drawInnerForPath(juce::Graphics& g, juce::Path target)
  {
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(target);
    juce::DropShadow ds(colour, radius, offset);
    ds.drawForPath(g, shadowPath);
  }
  void drawOuterForPath(juce::Graphics& g, juce::Path target)
  {
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(shadowPath);
    juce::DropShadow ds(colour, radius, offset);
    ds.drawForPath(g, target);
  }
  void drawForPath(juce::Graphics& g, juce::Path target)
  {
    int cycles = (int)radius;
    for (int i = 0; i < cycles; i++) {
      int amount = (int)(std::sqrt((float)i / (float)cycles) * 255.0f);
    }
  }
  juce::Colour colour = juce::Colours::black;
  juce::Point<int> offset = { 0, 0 };
  int radius = 10;
};
//==============================================================================
}