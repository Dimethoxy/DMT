//==============================================================================

#pragma once

#include "dsp/synth/AnalogOscillator.h"
#include "gui/widget/Shadow.h"
#include "utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class OscillatorDisplayComponent
  : public juce::Component
  , public juce::Timer
{
  using Shadow = dmt::gui::widget::Shadow;
  using AnalogOscillator = dmt::dsp::synth::AnalogOscillator;

  // General
  const int& fps = dmt::Settings::framerate;
  using Settings = dmt::Settings::OscillatorDisplay;
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
    table.initialise(
      [&](std::size_t index) { return (float)osc.getNextSample(); },
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
  Shadow outerShadow =
    Shadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false);
  Shadow innerShadow =
    Shadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true);

  AnalogOscillator osc;
  juce::dsp::LookupTable<float> table;
  juce::AudioProcessorValueTreeState& apvts;
};
} // namespace components
} // namespace gui
} // namespace dmt