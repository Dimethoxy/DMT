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
  inline const juce::String getName() noexcept override
  {
    return "Analog Gain Envelope";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
