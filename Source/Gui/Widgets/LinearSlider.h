#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {

enum LinearSliderType
{
  Positive,
  Negative,
  Bipolar,
  Selector
};

enum LinearSliderOrientation
{
  Horizontal,
  Vertical
};

class LinearSlider : public juce::Slider
{
  using Settings = dmt::AppSettings;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Slider::padding;

public:
  LinearSlider()
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
  LinearSliderType type = LinearSliderType::Positive;
};
} // namespace widgets
} // namespace gui
} // namespace dmt