#pragma once

#include "../../utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

class LinearSlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  using Slider = Settings::Slider;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;
  // Thumb
  const float& thumbSize = Slider::thumbSize;

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

  LinearSlider(Type /*type*/, Orientation /*orientation*/)
    : juce::Slider()
  {
    setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    juce::Random rand;
    setValue(10.0f * rand.nextFloat());
  }
  void paint(juce::Graphics& g) override
  {
    auto bounds = getLocalBounds();

    g.setColour(juce::Colours::red);
    g.fillRect(bounds);

    switch (orientation) {
      case Orientation::Horizontal:
        drawHorizontal();
        break;
      case Orientation::Vertical:
        drawVertical();
        break;
      default:
        jassert(false);
        break;
    }
  }
  void setBoundsByPoints(juce::Point<int> primaryPoint,
                         juce::Point<int> secondaryPoint)
  {
    const int padding = (int)(rawPadding * size);
    const float primaryX = (float)primaryPoint.getX();
    const float primaryY = (float)primaryPoint.getY();
    const float secondaryX = (float)primaryPoint.getX();
    const float secondaryY = (float)primaryPoint.getY();

    const float minSize = thumbSize * size;
    const float xDistance = secondaryX - primaryX;
    const float innerWidth = (minSize > xDistance) ? minSize : xDistance;
    const float yDistance = secondaryY - primaryY;
    const float innerHeight = (minSize > yDistance) ? minSize : yDistance;

    const auto innerBounds =
      juce::Rectangle<float>(primaryX, primaryY, innerWidth, innerHeight);
    const auto centre = innerBounds.getCentre();

    const auto outerBounds = innerBounds.expanded(padding).withCentre(centre);
    this->setBounds(outerBounds.toNearestInt());
  }

protected:
  void drawHorizontal() {}
  void drawVertical() {}

private:
  Type type;
  Orientation orientation;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSlider)
};
} // namespace widgets
} // namespace gui
} // namespace dmt