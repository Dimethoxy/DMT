//==============================================================================

#pragma once

#include "../Components/TitleTopComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
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
}
