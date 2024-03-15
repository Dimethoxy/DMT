#pragma once
//==============================================================================
#include "../component/OscilloscopeComponent.h"
#include "./Panel.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
template<typename SampleType>
class OscilloscopePanel : public dmt::gui::Panel
{
  using OscilloscopeProcessor =
    dmt::dsp::processor::OscilloscopeProcessor<SampleType>;
  using OscilloscopeComponent =
    dmt::gui::component::OscilloscopeComponent<SampleType>;

public:
  OscilloscopePanel(OscilloscopeProcessor& processor)
    : dmt::gui::Panel("Oscilloscope", false)
    , oscilloscopeComponent(processor)
  {
  }

private:
  OscilloscopeComponent oscilloscopeComponent;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt