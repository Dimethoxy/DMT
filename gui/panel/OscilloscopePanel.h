#pragma once
//==============================================================================
#include "gui/component/OscilloscopeComponent.h"
#include "gui/panel/AbstractPanel.h"
#include "utility/RepaintTimer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
template<typename SampleType>
class OscilloscopePanel : public dmt::gui::AbstractPanel
{
  using OscilloscopeComponent =
    dmt::gui::component::OscilloscopeComponent<SampleType>;

public:
  OscilloscopePanel()
    : dmt::gui::Panel("Oscilloscope", false)
    , oscilloscopeComponent()
  {
    addAndMakeVisible(oscilloscopeComponent);
  }

  void resized() noexcept override
  {
    Panel::resized();
    oscilloscopeComponent.setBounds(getLocalBounds().reduced(30));
  }

private:
  OscilloscopeComponent oscilloscopeComponent;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt