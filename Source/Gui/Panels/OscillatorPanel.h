//==============================================================================

#pragma once

#include "../Widgets/Carousel.h"
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
  {
    panels.push_back(std::make_unique<AnalogOscillatorPanel>());
    panels.push_back(std::make_unique<ModernOscillatorPanel>());
    init();
  }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
