//==============================================================================

#pragma once

#include "../Synth/AnalogOscillator.h"
#include "../Utility/AppSettings.h"
#include "../Utility/Shadow.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class OscillatorDisplayComponent : public juce::Component
{
  using Settings = dmt::AppSettings::OscillatorDisplay;
  const int resolution = 100;

public:
  //============================================================================
  OscillatorDisplayComponent()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
    osc.setSampleRate((float)resolution);
    osc.setFrequency(1.0f);
    buildTable();
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(Settings::backgroundColour);
    g.fillEllipse(bounds);

    const auto borderSize = Settings::borderSize;
    const auto borderBounds = bounds.reduced(Settings::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = Settings::outerCornerSize;
    const auto innerCornerSize = Settings::innerCornerSize;

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addEllipse(borderBounds);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    g.setColour(Settings::foregroundColour);
    g.fillEllipse(innerBounds);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addEllipse(innerBounds);
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
    g.drawEllipse(borderBounds.reduced(Settings::borderSize / 2.0f),
                  Settings::borderSize);
  }
  //============================================================================
  juce::Path getPath(juce::Rectangle<float> bounds)
  {
    bounds.setY(bounds.getY() + (bounds.getHeight() / 10.0f));
    bounds.setHeight(bounds.getHeight() - (bounds.getHeight() / 5.0f));

    auto outerBounds = bounds;
    bounds = bounds.reduced(bounds.getWidth() / 6.0f);

    juce::Path path;

    auto startX = bounds.getX();
    auto startY = bounds.getY() + (bounds.getHeight() / 2.0f);
    juce::Point<float> start(startX, startY);
    path.startNewSubPath(start);

    auto width = bounds.getWidth();

    for (int i = 0; i < width; i++) {
      auto x = bounds.getX() + i;
      auto y = bounds.getY() + (bounds.getHeight() / 2.0f) -
               (table[i / width * 100] * bounds.getHeight() / 2.0f);
      juce::Point<float> p(x, y);
      path.lineTo(p);
    }

    auto endX = bounds.getX() + bounds.getWidth();
    auto endY = bounds.getY() + (bounds.getHeight() / 2.0f);
    juce::Point<float> end(endX, endY);
    path.lineTo(end);

    return path;
  }
  //============================================================================

  void buildTable()
  {
    osc.setPhase(0.0f);
    table.initialise([&](size_t index) { return (float)osc.getNextSample(); },
                     resolution);
  }

  //============================================================================
private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  dmt::Shadow lineShadow;

  dmt::AnalogOscillator osc;
  juce::dsp::LookupTable<float> table;
  juce::Path graphPath;
  juce::Path integralPath;
};
//==============================================================================
}