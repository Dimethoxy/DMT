//==============================================================================

#pragma once

#include "../Utility/AppSettings.h"
#include "../Utility/Shadow.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class Panel : public juce::Component
{
public:
  using Settings = dmt::AppSettings::Panel;
  Panel()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(Settings::backgroundColour);
    g.fillRect(bounds);

    const auto borderSize = Settings::borderSize;
    const auto borderBounds = bounds.reduced(Settings::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = Settings::outerCornerSize;
    const auto innerCornerSize = Settings::innerCornerSize;

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    g.setColour(Settings::borderColour);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(Settings::foregroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, outerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
//==============================================================================
}
