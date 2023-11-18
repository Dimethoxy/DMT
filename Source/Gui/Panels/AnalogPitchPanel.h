//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class AnalogPitchPanel : public dmt::gui::Panel
{
public:
  AnalogPitchPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  inline const juce::String getName() noexcept override
  {
    return "Analog Pitch Envelope";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
