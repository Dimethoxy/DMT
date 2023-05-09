#pragma once

#include "../Utility/AppSettings.h"
#include "LookAndFeel.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
class LinearSlider : public juce::Slider
{
public:
  LinearSlider()
    : juce::Slider()
    , lnf()
  {
    setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    juce::Random rand;
    setValue(10.0f * rand.nextFloat());
    setLookAndFeel(&lnf);
  }
  ~LinearSlider() { setLookAndFeel(nullptr); }

private:
  dmt::LookAndFeel lnf;
};
} // namespace widgets
} // namespace gui
} // namespace dmt