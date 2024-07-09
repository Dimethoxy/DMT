//==============================================================================

#pragma once

#include "../widgets/Carousel.h"
#include "AnalogOscillatorPanel.h"
#include "ModernOscillatorPanel.h"
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
    : Carousel()
  {
    panels.push_back(std::make_unique<AnalogOscillatorPanel>());
    panels.push_back(std::make_unique<ModernOscillatorPanel>());
    init();
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt