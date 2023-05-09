//==============================================================================

#pragma once

#include "../Components/TitleTopComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class VoicePanel : public dmt::Panel
{
public:
  VoicePanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  void update() override {}
  juce::String getName() override { return "Voice"; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
