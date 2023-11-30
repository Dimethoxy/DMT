//==============================================================================
#pragma once
//==============================================================================
#include "../../utility/AppSettings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class LinearSlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  using Slider = Settings::Slider;
  using Colour = Settings::Colour;
  using StrokeType = juce::PathStrokeType;
  const float& size = Settings::Layout::size;
  // General
  const float& rawPadding = Settings::Slider::padding;
  // Shaft
  const juce::Colour& shaftColour = Slider::shaftColour;
  const float& rawShaftLineStrength = Slider::shaftLineStrength;
  const float& rawShaftSize = Slider::shaftSize;
  // Rail
  const juce::Colour& lowerRailColour = Slider::lowerRailColour;
  const juce::Colour& upperRailColour = Slider::upperRailColour;
  const float& rawRailWidth = Slider::railWidth;
  const float& railSize = Slider::railSize;
  // Thumb
  const juce::Colour& thumbInnerColour = Slider::thumbInnerColour;
  const juce::Colour& thumOuterColour = Slider::thumOuterColour;
  const float& rawThumbSize = Slider::thumbSize;
  const float& rawThumbStrength = Slider::thumbStrength;

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

  LinearSlider(const Type /*type*/, const Orientation orientation)
    : juce::Slider()
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
    const auto railWidth = rawRailWidth * size;
    const auto jointStyle = StrokeType::curved;
    const auto endCapStyle = StrokeType::butt;
    const auto strokeType = StrokeType(railWidth, jointStyle, endCapStyle);
    auto lowerRailPath = juce::Path();
    lowerRailPath.startNewSubPath(primaryPoint);
    lowerRailPath.lineTo(secondaryPoint);

    // Draw lower rail
    g.setColour(lowerRailColour);
    g.strokePath(lowerRailPath, strokeType);

    // Calculate upper rail
    const auto value = getValue();
    const auto maximum = getMaximum();
    const auto minimum = getMinimum();
    const auto range = maximum - minimum;
    const auto valueRatio = (value - minimum) / range;
    const auto diffrencePoint = secondaryPoint - primaryPoint;
    const auto valueDiffrencePoint = diffrencePoint * valueRatio;
    const auto upperRailPoint = primaryPoint + valueDiffrencePoint;
    auto upperRailPath = juce::Path();
    upperRailPath.startNewSubPath(primaryPoint);
    upperRailPath.lineTo(upperRailPoint);

    // Draw
    g.setColour(upperRailColour);
    g.strokePath(upperRailPath, strokeType);
  }

  void setBoundsPoints(juce::Point<int> newPrimaryPoint,
                       juce::Point<int> newSecondaryPoint)
  {
    primaryPoint = newPrimaryPoint.toFloat();
    secondaryPoint = newSecondaryPoint.toFloat();
  }

private:
  Type type;
  Orientation orientation;
  juce::Point<float> primaryPoint;
  juce::Point<float> secondaryPoint;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSlider)
};
} // namespace widgets
} // namespace gui
} // namespace dmt