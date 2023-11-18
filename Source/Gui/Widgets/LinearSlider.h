#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

class LinearSlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;

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

  LinearSlider(Type type, Orientation orientation)
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
    auto padding = rawPadding * size;
    auto reducedBounds = bounds.reduced(padding);

    g.setColour(juce::Colours::red);
    g.fillRect(bounds);
  }

private:
  Type type;
  Orientation orientation;
};
} // namespace widgets
} // namespace gui
} // namespace dmt