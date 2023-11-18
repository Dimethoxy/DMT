//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class VoicePanel : public dmt::gui::Panel
{
public:
  VoicePanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  juce::String getName() override { return "Hyper Oscillator"; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
