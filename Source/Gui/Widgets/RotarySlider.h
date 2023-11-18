#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

class RotarySlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;

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
    auto bounds = getLocalBounds();
    auto padding = rawPadding * size;
    auto reducedBounds = bounds.reduced(padding);

    g.setColour(juce::Colours::green);
    g.fillRect(bounds);
  }
  Type getType() { return type; };

private:
  Type type;
};
} // namespace widget
} // namespace gui
} // namespace dmt