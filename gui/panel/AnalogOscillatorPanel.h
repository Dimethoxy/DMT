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
    : Panel("Classic Oscillator")
  { //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogOscillatorPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
