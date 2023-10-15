//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernGainPanel : public dmt::gui::Panel
{
public:
  ModernGainPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  juce::String getName() override { return "Analog Gain Panel"; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt