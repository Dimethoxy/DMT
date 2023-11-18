/*
  ==============================================================================

        RotarySlider.h
        Created: 26 May 2022 12:00:24pm
        Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../../Utility/AppSettings.h"
#include "./LookAndFeel.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widget {

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
    auto margin = 1.0;
  }
  ~RotarySlider() { setLookAndFeel(nullptr); }
  dmt::gui::widget::RotarySliderType getType() { return type; };

private:
  RotarySliderType type = RotarySliderType::Positive;
};
} // namespace widget
} // namespace gui
} // namespace dmt