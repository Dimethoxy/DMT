//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class VoicingPanel : public dmt::gui::Panel
{
public:
  VoicingPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  inline const juce::String getName() noexcept override { return "Voices"; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
