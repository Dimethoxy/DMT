//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernPitchPanel : public dmt::gui::Panel
{
public:
  ModernPitchPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  inline const juce::String getName() noexcept override
  {
    return "Modern Pitch Panel";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt