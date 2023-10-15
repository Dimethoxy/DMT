//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class OscillatorPanel : public dmt::gui::Panel
{
public:
  OscillatorPanel()
  { //
  }
  juce::String getName() override { return "Voices"; }
  bool isSelector() { return true; }

private:
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
