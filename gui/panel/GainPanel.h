//==============================================================================

#pragma once

#include "../Widgets/Carousel.h"
#include "AnalogGainPanel.h"
#include "ModernGainPanel.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class GainPanel : public dmt::gui::panels::Carousel
{
public:
  GainPanel(juce::AudioProcessorValueTreeState& apvts)
    : Carousel()
  {
    panels.push_back(std::make_unique<AnalogGainPanel>(apvts));
    panels.push_back(std::make_unique<ModernGainPanel>());
    init();
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
