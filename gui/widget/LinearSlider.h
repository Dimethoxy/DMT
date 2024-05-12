//==============================================================================
#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class LinearSlider : public juce::Slider
{
  using Settings = dmt::Settings;
  using Colour = Settings::Colour;
  using StrokeType = juce::PathStrokeType;
  const float& size = Settings::Layout::size;
  // General
  const float& rawPadding = Settings::Slider::padding;
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

public:
  enum Type
  {
    Positive,
    Negative,
    Bipolar,
    Selector
  };

  enum Orientation
  {
    Horizontal,
    Vertical
  };

  LinearSlider(const Type type, const Orientation orientation)
    : juce::Slider()
    , type(type)
    , orientation(orientation)
  {
    switch (orientation) {
      case Orientation::Horizontal: {
        setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
        break;
      }
      case Orientation::Vertical: {
        setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        break;
      }
      default: {
        break;
      }
    }
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    juce::Random rand;
    setValue(10.0f * rand.nextFloat());
  }

  void paint(juce::Graphics& g) override
  {
    auto bounds = getLocalBounds();

    // Draw bounds debug
    g.setColour(juce::Colours::cyan);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1);

    // Calculate lower rail
    float thumbSize = rawThumbSize * size;
    const auto padding = rawPadding * size;
    const auto railBounds = bounds.reduced(thumbSize / 2.0f);
    float primaryPointX;
    float primaryPointY;
    float secondaryPointX;
    float secondaryPointY;
    switch (orientation) {
      case Orientation::Horizontal:
        primaryPointX = railBounds.getX();
        primaryPointY = railBounds.getCentreY();
        secondaryPointX = railBounds.getRight();
        secondaryPointY = railBounds.getCentreY();
        break;
      case Orientation::Vertical:
        primaryPointX = railBounds.getCentreX();
        primaryPointY = railBounds.getBottom();
        secondaryPointX = railBounds.getCentreX();
        secondaryPointY = railBounds.getY();
        break;
      default:
        jassert(false);
        return;
    }
    const juce::Point<float> primaryPoint(primaryPointX, primaryPointY);
    const juce::Point<float> secondaryPoint(secondaryPointX, secondaryPointY);

    const auto railWidth = rawRailWidth * size;
    const auto jointStyle = StrokeType::curved;
    const auto lowerEndCapStyle = StrokeType::rounded;
    const auto lowerStrokeType =
      StrokeType(railWidth, jointStyle, lowerEndCapStyle);
    auto lowerRailPath = juce::Path();
    lowerRailPath.startNewSubPath(primaryPoint);
    lowerRailPath.lineTo(secondaryPoint);

    // Draw lower rail
    g.setColour(lowerRailColour);
    g.strokePath(lowerRailPath, lowerStrokeType);

    // Calculate upper rail
    const auto upperEndCapStyle =
      (type == Type::Bipolar) ? StrokeType::butt : StrokeType::rounded;
    const auto upperStrokeType =
      StrokeType(railWidth, jointStyle, upperEndCapStyle);
    const auto value = getValue();
    const auto maximum = getMaximum();
    const auto minimum = getMinimum();
    const auto range = maximum - minimum;
    const auto valueRatio = (value - minimum) / range;
    const auto diffrencePoint = secondaryPoint - primaryPoint;
    const auto valueDiffrencePoint = diffrencePoint * valueRatio;
    const auto valuePoint = primaryPoint + valueDiffrencePoint;
    const auto middlePoint = (primaryPoint + secondaryPoint) / 2.0f;
    auto upperRailStartPoint = primaryPoint;
    switch (type) {
      case Type::Positive:
        upperRailStartPoint = primaryPoint;
        break;
      case Type::Negative:
        upperRailStartPoint = secondaryPoint;
        break;
      case Type::Bipolar:
        upperRailStartPoint = middlePoint;
        break;
    }
    const auto upperRailEndPoint = valuePoint;
    auto upperRailPath = juce::Path();
    upperRailPath.startNewSubPath(upperRailStartPoint);
    upperRailPath.lineTo(upperRailEndPoint);

    // Draw upper rail
    g.setColour(upperRailColour);
    g.strokePath(upperRailPath, upperStrokeType);

    // Draw the Thumb
    const auto thumbPoint = valuePoint;
    if (!isMouseButtonDown())
      thumbSize *= 0.85f;
    const float thumbStrength = rawThumbStrength * size;
    const auto thumbBounds = juce::Rectangle<float>()
                               .withSize(thumbSize, thumbSize)
                               .withCentre(thumbPoint);
    g.setColour(thumOuterColour);
    g.fillEllipse(thumbBounds);
    g.setColour(thumbInnerColour);
    g.fillEllipse(thumbBounds.reduced(thumbStrength));
  }

private:
  Type type;
  Orientation orientation;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSlider)
};
} // namespace widgets
} // namespace gui
} // namespace dmt