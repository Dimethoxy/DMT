//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernGainPanel : dmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

public:
  ModernGainPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : Panel("Modern Gain Envelope")
  {
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModernGainPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt