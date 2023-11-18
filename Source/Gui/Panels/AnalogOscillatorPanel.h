//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class AnalogOscillatorPanel : public dmt::gui::Panel
{
public:
  AnalogOscillatorPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  inline const juce::String getName() noexcept override
  {
    return "Analog Oscillator Envelope";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
