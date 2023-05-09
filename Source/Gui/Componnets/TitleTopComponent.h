//==============================================================================

#pragma once

#include "../Utility/AppSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace components {
//==============================================================================
class TitleTopComponent : public juce::Component
{
  using Settings = dmt::AppSettings::OscillatorTop;

public:
  TitleTopComponent(juce::String& name)
    : name(name)
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
    fontShadow.radius = Settings::outerShadowRadius;
    fontShadow.colour = Settings::fontShadowColour;
  }
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    auto innerBounds = bounds.withHeight(bounds.getHeight() * 0.6f);
    auto cornerSize = dmt::AppSettings::Panel::innerCornerSize;

    g.setColour(dmt::AppSettings::Colours::topground);
    g.fillRoundedRectangle(innerBounds, cornerSize);

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(innerBounds, cornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }
    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, cornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }

    juce::Font font =
      (AppSettings::Fonts::medium.withHeight(getHeight() * 0.5f));
    auto x = bounds.getX();
    auto y = bounds.getY() - bounds.getHeight() / 6.5f;
    auto w = bounds.getWidth();
    auto h = bounds.getHeight();
    juce::Path textPath;
    juce::GlyphArrangement glyphs;
    glyphs.addFittedText(
      font, "Classic Oscillator", x, y, w, h, juce::Justification::centred, 2);
    glyphs.createPath(textPath);

    if (Settings::drawFontShadow) {
      fontShadow.drawOuterForPath(g, textPath);
    }
    if (Settings::drawFontOutline) {
      g.setColour(Settings::fontOutlineColour);
      juce::PathStrokeType strokeType(Settings::fontOutlineThickness);
      g.strokePath(textPath, strokeType);
    }

    g.setColour(Settings::fontColour);
    g.fillPath(textPath);
  }

private:
  juce::String& name;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  dmt::Shadow fontShadow;
};
//==============================================================================
} // namespace dmt
} // namespace gui
} // namespace components