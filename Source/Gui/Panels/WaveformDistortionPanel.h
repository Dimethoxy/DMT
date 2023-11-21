//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class WaveformDistortionPanel : public dmt::gui::Panel
{
public:
  WaveformDistortionPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : Panel()
  { //
  }
  inline const juce::String getName() noexcept override
  {
    return "Waveform Distortion";
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDistortionPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
