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
  { //
  }
  juce::String getName() override { return "Analog Gain Panel"; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
