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
  AnalogPitchPanel(/*juce::AudioProcessorValueTreeState& apvts*/) //
    : Panel("Analog Pitch Envelope")
  { //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogPitchPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
