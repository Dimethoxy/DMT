#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

enum class RotarySliderType
{
  Positive,
  Negative,
  Bipolar,
  Selector
};

class RotarySlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;

public:
  RotarySlider(dmt::AppSettings& a, RotarySliderType type)
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
    auto bounds = getLocalBounds();
    auto padding = rawPadding * size;
    auto reducedBounds = bounds.reduced(padding);

    g.setColour(juce::Colours::green);
    g.fillRect(bounds);
  }
  RotarySliderType getType() { return type; };

private:
  RotarySliderType type = RotarySliderType::Positive;
};
} // namespace widget
} // namespace gui
} // namespace dmt