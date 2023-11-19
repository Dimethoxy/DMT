#pragma once
//==============================================================================
#include "../../Utility/AppSettings.h"
#include "../../Utility/Math.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class RotarySlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  using Slider = Settings::Slider;
  using Colour = Settings::Colour;
  using StrokeType = juce::PathStrokeType;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;
  const juce::Colour& shaftColour = Slider::shaftColour;
  const float& rawShaftLineStrength = Slider::shaftLineStrength;
  const float& rawShaftSize = Slider::shaftSize;
  const float& railWidth = Slider::railWidth;
  const float& railSize = Slider::railSize;

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
    setSkewFactor(2.0);
  }
  void paint(juce::Graphics& g) override
  {
    auto bounds = getLocalBounds().toFloat();
    auto padding = rawPadding * size;
    auto reducedBounds = bounds.reduced(padding);

    g.setColour(juce::Colours::green);
    g.drawRect(bounds, 1.0f);

    drawSlider(g, bounds);
  }
  Type getType() { return type; };

private:
  Type type;
  //============================================================================
  // Main Draw Function
  void drawSlider(juce::Graphics& g,
                  const juce::Rectangle<float>& bounds) const noexcept
  {
    // Draw bounds debug
    g.setColour(juce::Colours::aqua);
    g.drawRect(bounds, 1.0f);

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
    const float normalizedStartAngle = 0.0f;
    const float normalizedEndAngle = 270.0f;
    const float angleRange = normalizedEndAngle - normalizedStartAngle;
    const float gapRange = 360.0f - angleRange;
    const float angleOffset = 90.0f + (gapRange / 2.0f);
    const auto& centre = reducedBounds.getCentre();
    const auto tickLine = getTick(reducedBounds,
                                  centre,
                                  value,
                                  minValue,
                                  maxValue,
                                  normalizedStartAngle,
                                  normalizedEndAngle,
                                  angleOffset);
    g.drawLine(tickLine, lineStrength);

    // Draw the rail
    const auto jointStyle = StrokeType::curved;
    const auto endCapStyle = StrokeType::butt;
    const auto strokeType = StrokeType(railWidth, jointStyle, endCapStyle);
    const auto railBounds = bounds;
    const auto railRadius = railBounds.getWidth() * railSize / 2.0f;
    const auto startAngleRadians =
      dmt::Math::degreeToRadians(normalizedStartAngle - angleOffset);
    const auto endAngleRadians =
      dmt::Math::degreeToRadians(normalizedEndAngle - angleOffset);
    const auto lowerRail =
      getLowerRail(railBounds, railRadius, startAngleRadians, endAngleRadians);
    g.strokePath(lowerRail, strokeType);
  }
  //============================================================================
  const juce::Path getValueArcPath(juce::Rectangle<float> bounds,
                                   float arcRadius,
                                   float startAngle,
                                   float endAngle,
                                   float toAngle,
                                   Type type) const noexcept
  {
    juce::Path arc;
    switch (type) {
      case Type::Positive:
        arc.addCentredArc(bounds.getCentreX(),
                          bounds.getCentreY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          startAngle,
                          toAngle,
                          true);
        break;
      case Type::Negative:
        arc.addCentredArc(bounds.getCentreX(),
                          bounds.getCentreY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          endAngle,
                          toAngle,
                          true);
        break;
      case Type::Bipolar:
        auto center = (endAngle - startAngle) / 2.0f;
        arc.addCentredArc(bounds.getCentreX(),
                          bounds.getCentreY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          startAngle + center,
                          toAngle,
                          true);
        break;
    }
    return arc;
  }
  //============================================================================
  const juce::Path getLowerRail(juce::Rectangle<float> bounds,
                                float arcRadius,
                                float startAngleRadians,
                                float endAngleRadians) const noexcept
  {
    juce::Path arc;
    arc.addCentredArc(bounds.getCentreX(),
                      bounds.getCentreY(),
                      arcRadius,
                      arcRadius,
                      0.0f,
                      startAngleRadians,
                      endAngleRadians,
                      true);
    return arc;
  }
  //============================================================================
  const juce::Line<float> getTick(const juce::Rectangle<float>& bounds,
                                  const juce::Point<float>& centre,
                                  const float& value,
                                  const float& minValue,
                                  const float& maxValue,
                                  const float& startAngle,
                                  const float& endAngle,
                                  const float& angleOffset) const noexcept
  {
    const float rawAngle =
      juce::jmap(value, minValue, maxValue, startAngle, endAngle);
    const float angleInRadians =
      dmt::Math::degreeToRadians(rawAngle + angleOffset);
    const float outerRadius = bounds.getWidth() / 2.0f;
    const auto outerPoint =
      dmt::Math::pointOnCircle(centre, outerRadius, angleInRadians);
    const float innerRadius = outerRadius / 5.0f;
    const auto innerPoint =
      dmt::Math::pointOnCircle(centre, innerRadius, angleInRadians);
    return juce::Line<float>(outerPoint, innerPoint);
  }
  //============================================================================
  void drawSelector(juce::Rectangle<float> bounds) noexcept {}
  void drawSelectorThumb(juce::Rectangle<float> bounds) noexcept {}

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySlider)
};
} // namespace widget
} // namespace gui
} // namespace dmt