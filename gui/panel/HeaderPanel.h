//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class VoicePanel : dmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

public:
  VoicePanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  juce::String getName() override
  {
    return "Voices";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
