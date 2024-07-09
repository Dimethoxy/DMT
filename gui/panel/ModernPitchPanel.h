//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernPitchPanel : dmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
public:
  ModernPitchPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : Panel("Modern Pitch Envelope")
  {
    //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModernPitchPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt