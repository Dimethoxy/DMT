#pragma once

#include "./Panel.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
class OscilloscopePanel : public dmt::gui::Panel
{
public:
  OscilloscopePanel()
    : dmt::gui::Panel("Oscilloscope")
  {
    // Constructor code goes here
  }

private:
  // Private member variables go here

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt