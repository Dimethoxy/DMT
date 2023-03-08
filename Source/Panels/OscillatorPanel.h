//==============================================================================

#pragma once

#include "../Components/OscillatorDisplayComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel() { addAndMakeVisible(oscDisplay); }
  void resized() override
  {
    oscDisplay.setBoundsRelative(0.2f, 0.15f, 0.6f, 0.2f);
  }

private:
  dmt::OscillatorDisplayComponent oscDisplay;
};
//==============================================================================
}
