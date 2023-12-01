//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class AnalogGainPanel : public dmt::gui::Panel
{
public:
  AnalogGainPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : Panel("Analog Gain Envelope")
  { //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogGainPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
