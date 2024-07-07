#pragma once
//==============================================================================
#include "../../utility/Math.h"
#include "../../utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class RotarySlider : public juce::Slider
{
  using Settings = dmt::Settings;
  using Colour = Settings::Colour;
  using StrokeType = juce::PathStrokeType;
  const float& size = Settings::Layout::size;
  // General
  const float& rawPadding = Settings::Settings::Slider::padding;
  // Shaft
  const juce::Colour& shaftColour = Settings::Slider::shaftColour;
  const float& rawShaftLineStrength = Settings::Slider::shaftLineStrength;
  const float& rawShaftSize = Settings::Slider::shaftSize;
  // Rail
  const juce::Colour& lowerRailColour = Settings::Slider::lowerRailColour;
  const juce::Colour& upperRailColour = Settings::Slider::upperRailColour;
  const float& rawRailWidth = Settings::Slider::railWidth;
  const float& railSize = Settings::Slider::railSize;
  // Thumb
  const juce::Colour& thumbInnerColour = Settings::Slider::thumbInnerColour;
  const juce::Colour& thumOuterColour = Settings::Slider::thumOuterColour;
  const float& rawThumbSize = Settings::Slider::thumbSize;
  const float& rawThumbStrength = Settings::Slider::thumbStrength;
  // Selections
  const juce::Colour& selectionOuterColour =
    Settings::Slider::selectionOuterColour;
  const juce::Colour& selectionInnerColour =
    Settings::Slider::selectionInnerColour;
  const juce::Colour& selectionActiveColour =
    Settings::Slider::selectionActiveColour;
  const float& rawSelectionWidth = Settings::Slider::selectionWidth;
  const float& rawSelectionSize = Settings::Slider::selectionSize;
  const float& rawSelectionActivePadding =
    Settings::Slider::selectionActivePadding;
  //==============================================================================
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
    const auto bounds = getLocalBounds().toFloat();
    const auto padding = rawPadding * size;

    // Draw bounds debug
    g.setColour(juce::Colours::yellow);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1.0f);

    drawSlider(g, bounds.reduced(padding));
  }
  Type getType() { return type; };

protected:
  void drawSlider(juce::Graphics& g,
                  const juce::Rectangle<float>& bounds) const noexcept
  {
    TRACER("RotarySlider::drawSlider");
    // Draw bounds debug
    g.setColour(juce::Colours::aqua);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1.0f);

    // Draw the shaft
    const auto shaftSize = rawShaftSize * bounds.getHeight();
    auto shaftBounds = bounds;
    shaftBounds.setSize(shaftSize, shaftSize);
    shaftBounds.setCentre(bounds.getCentre());
    const float lineStrength = rawShaftLineStrength * size;
    const auto rawCentre = shaftBounds.getCentre();
    const float centreOffset = shaftBounds.getHeight() / 6.5f;
    const float centreY = rawCentre.getY() + centreOffset;
    const auto centre = juce::Point<float>(rawCentre.getX(), centreY);
    shaftBounds.setCentre(centre);
    g.setColour(shaftColour);
    g.drawEllipse(shaftBounds, lineStrength);

    // Draw the tick
    const float value = (float)getValue();
    const float minValue = (float)getMinimum();
    const float maxValue = (float)getMaximum();
    const float normalizedStartAngle = 0.0f;
    const float normalizedEndAngle = 270.0f;
    const float angleRange = normalizedEndAngle - normalizedStartAngle;
    const float gapRange = 360.0f - angleRange;
    const float angleOffset = 180.0f + (gapRange / 2.0f);
    const float rawAngle = juce::jmap(
      value, minValue, maxValue, normalizedStartAngle, normalizedEndAngle);
    const float valueAngleInRadians =
      dmt::math::degreeToRadians(rawAngle + angleOffset);
    const auto tickLine = getTick(shaftBounds, centre, valueAngleInRadians);
    g.drawLine(tickLine, lineStrength);

    // Rail and selector
    const auto railBounds = bounds;
    const auto railRadius = railBounds.getWidth() * railSize / 2.0f;

    // Draw the lower rail
    if (type != Type::Selector) {
      const auto railWidth = rawRailWidth * size;
      const auto jointStyle = StrokeType::curved;
      const auto endCapStyle = StrokeType::rounded;
      const auto strokeType = StrokeType(railWidth, jointStyle, endCapStyle);
      const auto startAngleInRadians =
        dmt::math::degreeToRadians(normalizedStartAngle + angleOffset);
      const auto endAngleInRadians =
        dmt::math::degreeToRadians(normalizedEndAngle + angleOffset);
      const auto lowerRail = getLowerRail(
        centre, railRadius, startAngleInRadians, endAngleInRadians);
      g.setColour(lowerRailColour);
      g.strokePath(lowerRail, strokeType);

      // Draw the upper rail
      const auto upperRail = getUpperRail(centre,
                                          railRadius,
                                          startAngleInRadians,
                                          endAngleInRadians,
                                          valueAngleInRadians);
      g.setColour(upperRailColour);
      g.strokePath(upperRail, strokeType);
    } else {
      const int numSelections = (int)maxValue - (int)minValue;
      for (int i = 0; i <= numSelections; i++) {
        const float selectionValue = minValue + i;
        const float rawSelectionAngle = juce::jmap(selectionValue,
                                                   minValue,
                                                   maxValue,
                                                   normalizedStartAngle,
                                                   normalizedEndAngle);
        const float selectionAngleInRadians =
          dmt::math::degreeToRadians(rawSelectionAngle + angleOffset);
        const auto slectionCentre =
          dmt::math::pointOnCircle(centre, railRadius, selectionAngleInRadians);
        const float selectionSize = rawSelectionSize * size;
        const auto selectionBounds = juce::Rectangle<float>()
                                       .withSize(selectionSize, selectionSize)
                                       .withCentre(slectionCentre);
        g.setColour(selectionOuterColour);
        g.fillEllipse(selectionBounds);

        const auto selectionInnerBounds =
          selectionBounds.reduced(rawSelectionWidth * size);

        g.setColour(selectionInnerColour);
        g.fillEllipse(selectionInnerBounds);
      }
    }

    // Draw the Thumb
    const auto thumbPoint =
      dmt::math::pointOnCircle(centre, railRadius, valueAngleInRadians);
    float thumbSize = rawThumbSize * size;
    if (!isMouseButtonDown())
      thumbSize *= 0.85f;
    const float thumbStrength = rawThumbStrength * size;
    const auto thumbBounds = juce::Rectangle<float>()
                               .withSize(thumbSize, thumbSize)
                               .withCentre(thumbPoint);
    const auto thumbInnerBounds = thumbBounds.reduced(thumbStrength);
    g.setColour(thumOuterColour);
    g.fillEllipse(thumbBounds);
    g.setColour(thumbInnerColour);
    g.fillEllipse(thumbInnerBounds);

    if (type == Type::Selector) {
      const float activePadding = rawSelectionActivePadding * size;
      const auto activeBounds = thumbInnerBounds.reduced(activePadding);
      g.setColour(selectionActiveColour);
      g.fillEllipse(activeBounds);
    }
  }
  //============================================================================
  const juce::Path getUpperRail(const juce::Point<float>& centre,
                                const float arcRadius,
                                const float startAngleInRadians,
                                const float endAngleInRadians,
                                const float valueAngleInRadians) const noexcept
  {
    juce::Path arc;
    switch (this->type) {
      case Type::Selector:
        jassert("How did we get here?");
      case Type::Positive:
        arc.addCentredArc(centre.getX(),
                          centre.getY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          startAngleInRadians,
                          valueAngleInRadians,
                          true);
        break;
      case Type::Negative:
        arc.addCentredArc(centre.getX(),
                          centre.getY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          endAngleInRadians,
                          valueAngleInRadians,
                          true);
        break;
      case Type::Bipolar:
        auto centreValue = (endAngleInRadians - startAngleInRadians) / 2.0f;
        arc.addCentredArc(centre.getX(),
                          centre.getY(),
                          arcRadius,
                          arcRadius,
                          0.0f,
                          startAngleInRadians + centreValue,
                          valueAngleInRadians,
                          true);
        break;
    }
    return arc;
  }

  //============================================================================
  const juce::Path getLowerRail(const juce::Point<float>& centre,
                                float arcRadius,
                                float startAngleInRadians,
                                float endAngleInRadians) const noexcept
  {
    juce::Path arc;
    arc.addCentredArc(centre.getX(),
                      centre.getY(),
                      arcRadius,
                      arcRadius,
                      0.0f,
                      startAngleInRadians,
                      endAngleInRadians,
                      true);
    return arc;
  }
  //============================================================================
  const juce::Line<float> getTick(const juce::Rectangle<float>& bounds,
                                  const juce::Point<float>& centre,
                                  const float& angleInRadians) const noexcept
  {
    const float outerRadius = bounds.getWidth() / 2.0f;
    const auto outerPoint =
      dmt::math::pointOnCircle(centre, outerRadius, angleInRadians);
    const float innerRadius = outerRadius / 5.0f;
    const auto innerPoint =
      dmt::math::pointOnCircle(centre, innerRadius, angleInRadians);
    return juce::Line<float>(outerPoint, innerPoint);
  }
  //============================================================================
private:
  Type type;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySlider)
};
} // namespace widgets
} // namespace gui
} // namespace dmt
