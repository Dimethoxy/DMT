//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernOscillatorPanel : public dmt::gui::Panel
{
public:
  ModernOscillatorPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
  { //
  }
  inline const juce::String getName() noexcept override
  {
    return "Modern Oscillator Panel";
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt