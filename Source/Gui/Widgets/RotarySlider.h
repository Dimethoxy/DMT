#pragma once

#include "../../Utility/AppSettings.h"
#include "../../Utility/Math.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

class RotarySlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  using Slider = Settings::Slider;
  using Colour = Settings::Colour;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;
  const juce::Colour& shaftColour = Slider::shaftColour;
  const float& shaftLineStrength = Slider::shaftLineStrength;
  const float& shaftSize = Slider::shaftSize;

public:
  enum class Type
  {
    Positive,
    Negative,
    Bipolar,
    Selector
  };
  RotarySlider(Type type)
    : juce::Slider()
    , type(type)
  {
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    juce::Random rand;
    setValue(10.0f * rand.nextFloat());
  }
  void paint(juce::Graphics& g) override
  {
    auto bounds = getLocalBounds().toFloat();
    auto padding = rawPadding * size;
    auto reducedBounds = bounds.reduced(padding);

    g.setColour(juce::Colours::green);
    g.drawRect(bounds, 1.0f);

    drawShaft(g, bounds);
  }
  Type getType() { return type; };

private:
  Type type;

  void drawShaft(juce::Graphics& g, juce::Rectangle<float>& bounds) noexcept
  {
    float lineStrength = shaftLineStrength * size;
    auto reducedBounds = bounds.reduced(lineStrength);
    reducedBounds.setWidth(reducedBounds.getHeight());
    reducedBounds.setCentre(bounds.getCentre());
    g.setColour(shaftColour);
    g.drawEllipse(reducedBounds, lineStrength);
  }

  void drawRail(juce::Rectangle<float> bounds) noexcept {}
  void drawRailThumb(juce::Rectangle<float> bounds) noexcept {}
  void drawSelector(juce::Rectangle<float> bounds) noexcept {}
  void drawSelectorThumb(juce::Rectangle<float> bounds) noexcept {}

  // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySlider)
};
} // namespace widget
} // namespace gui
} // namespace dmt