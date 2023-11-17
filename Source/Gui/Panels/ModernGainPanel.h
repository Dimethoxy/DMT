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
  inline const juce::String getName() noexcept override
  {
    return "Modern Gain Panel";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt