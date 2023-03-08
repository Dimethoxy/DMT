//==============================================================================

#pragma once

#include "../Utility/AppSettings.h"
#include "../Utility/Shadow.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class OscillatorDisplayComponent : public juce::Component
{
  using Settings = dmt::AppSettings::OscillatorDisplay;

public:
  //============================================================================
  OscillatorDisplayComponent()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  //============================================================================
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

    g.setColour(Settings::foregroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, outerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }

    auto graphPath = getPath(borderBounds.reduced(Settings::graphSize / 2.0f));
    auto integralPath = graphPath;
    integralPath.closeSubPath();

    g.setColour(Settings::integralColour);
    g.fillPath(integralPath);

    g.setColour(Settings::graphColor);
    g.strokePath(graphPath, juce::PathStrokeType(Settings::graphSize));

    g.setColour(Settings::borderColour);
    g.drawRoundedRectangle(borderBounds.reduced(Settings::borderSize / 2.0f),
                           outerCornerSize,
                           Settings::borderSize);
  }
  //============================================================================
  juce::Path getPath(juce::Rectangle<float> bounds)
  {
     bounds.setY(bounds.getY() + (bounds.getHeight() / 10.0f));
     bounds.setHeight(bounds.getHeight() - (bounds.getHeight() / 5.0f));
    juce::Path path;

    auto startX = bounds.getX();
    auto startY = bounds.getY() + (bounds.getHeight() / 2.0f);
    juce::Point<float> start(startX, startY);
    path.startNewSubPath(start);

    auto p1x = bounds.getX() + bounds.getWidth() / 4.0f;
    auto p1y = bounds.getY();
    juce::Point<float> p1(p1x, p1y);
    path.lineTo(p1);

    auto p2x = bounds.getX() + bounds.getWidth() / 4.0f * 3.0f;
    int p2y = std::floor(bounds.getY() + bounds.getHeight());
    juce::Point<float> p2(p2x, p2y);
    path.lineTo(p2);

    auto endX = bounds.getX() + bounds.getWidth();
    auto endY = bounds.getY() + (bounds.getHeight() / 2.0f);
    juce::Point<float> end(endX, endY);
    path.lineTo(end);

    return path;
  }
  //============================================================================
private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  dmt::Shadow lineShadow;
};
//==============================================================================
}