//==============================================================================

#pragma once

#include "dsp/Synth/AnalogOscillator.h"
#include "gui/widgets/Shadow.h"
#include "utility/AppSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace components {
//==============================================================================
class OscillatorDisplayComponent
  : public juce::Component
  , public juce::Timer
{
  // General
  using Settings = dmt::AppSettings::OscillatorDisplay;
  const int& fps = dmt::AppSettings::fps;
  const int& resolution = Settings::resolution;
  // Shadows
  const bool& drawOuterShadow = Settings::drawOuterShadow;
  const bool& drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour& outerShadowColour = Settings::outerShadowColour;
  const juce::Colour& innerShadowColour = Settings::innerShadowColour;
  const float& outerShadowRadius = Settings::outerShadowRadius;
  const float& innerShadowRadius = Settings::innerShadowRadius;

public:
  //============================================================================
  OscillatorDisplayComponent(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
  {
    osc.setSampleRate((float)resolution + 1.0f);
    osc.setFrequency(1.0f);
    startTimerHz(60);
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
  }

protected:
  //==============================================================================
  void timerCallback()
  {
    if (isParametersChanged()) {
      this->buildTable();
      this->repaint();
    }
  }

  void buildTable()
  {
    osc.setPhase(0.0f);
    table.initialise([&](size_t index) { return (float)osc.getNextSample(); },
                     resolution);
  }

  bool isParametersChanged() {}

  //==============================================================================
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
               (table[i / width * resolution] * bounds.getHeight() / 2.0f);
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
private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;

  dmt::dsp::synth::AnalogOscillator osc;
  juce::dsp::LookupTable<float> table;
  juce::AudioProcessorValueTreeState& apvts;
};
} // namespace components
} // namespace gui
} // namespace dmt