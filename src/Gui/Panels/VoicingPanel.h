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
    : Panel("Voices")
  { //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicingPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
