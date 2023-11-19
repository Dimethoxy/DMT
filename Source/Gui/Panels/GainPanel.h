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
class OscillatorPanel : public dmt::gui::panels::Carousel
{
public:
  OscillatorPanel()
  {
    panels.push_back(std::make_unique<AnalogGainPanel>());
    panels.push_back(std::make_unique<ModernGainPanel>());
    init();
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
