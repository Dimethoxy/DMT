//==============================================================================

#pragma once

#include "gui/panel/AbstractPanel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class ModernOscillatorPanel : dmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

public:
  ModernOscillatorPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : Panel("Modern Oscillator")
  { //
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModernOscillatorPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt