//==============================================================================

#pragma once

#include "../widgets/Carousel.h"
#include "AnalogPitchPanel.h"
#include "ModernPitchPanel.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class PitchPanel : public dmt::gui::panels::Carousel
{
public:
  PitchPanel(juce::AudioProcessorValueTreeState& apvts)
    : Carousel()
  {
    panels.push_back(std::make_unique<AnalogPitchPanel>(apvts));
    panels.push_back(std::make_unique<ModernPitchPanel>());
    init();
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
