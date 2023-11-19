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
  const float& rawShaftLineStrength = Slider::shaftLineStrength;
  const float& rawShaftSize = Slider::shaftSize;

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

  void drawShaft(juce::Graphics& g,
                 const juce::Rectangle<float>& bounds) const noexcept
  {
    // Draw the circle
    const auto shaftSize = rawShaftSize * bounds.getHeight();
    auto reducedBounds = juce::Rectangle<float>(bounds);
    reducedBounds.setSize(shaftSize, shaftSize);
    reducedBounds.setCentre(bounds.getCentre());
    const float lineStrength = rawShaftLineStrength * size;
    g.setColour(shaftColour);
    g.drawEllipse(reducedBounds, lineStrength);

    // Draw the tick
    const float& value = getValue();
    const float& minValue = getMinimum();
    const float& maxValue = getMaximum();
    const float startAngle = 0.0f;
    const float endAngle = 270.0f;
    const float rawAngle =
      juce::jmap(value, minValue, maxValue, startAngle, endAngle);
    const float angleRange = endAngle - startAngle;
    const float gapRange = 360.0f - angleRange;
    const float angleOffset = 90.0f + (gapRange / 2.0f);
    const float angleInRadians =
      dmt::Math::degreeToRadians(rawAngle + angleOffset);
    const float radius = reducedBounds.getWidth() / 2.0f;
    const auto& center = reducedBounds.getCentre();
    const auto outerPoint =
      dmt::Math::pointOnCircle(center, radius, angleInRadians);

    const juce::Line<float> tickLine(outerPoint, center);
    g.drawLine(tickLine, lineStrength);
  }

  void drawRail(juce::Rectangle<float> bounds) noexcept {}
  void drawRailThumb(juce::Rectangle<float> bounds) noexcept {}
  void drawSelector(juce::Rectangle<float> bounds) noexcept {}
  void drawSelectorThumb(juce::Rectangle<float> bounds) noexcept {}

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySlider)
};
} // namespace widget
} // namespace gui
} // namespace dmt