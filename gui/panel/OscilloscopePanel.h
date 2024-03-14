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
  {
    // Constructor code goes here
  }

  void paint(juce::Graphics& g) override
  {
    // Paint code goes here
  }

  void resized() override
  {
    // Resizing code goes here
  }

private:
  // Private member variables go here

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt