#pragma once
//==============================================================================
#include "gui/component/OscilloscopeComponent.h"
#include "gui/panel/AbstractPanel.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
template<typename SampleType>
class OscilloscopePanel : public AbstractPanel
{
  using OscilloscopeComponent =
    dmt::gui::component::OscilloscopeComponent<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

public:
  OscilloscopePanel(FifoAudioBuffer& fifoBuffer)
    : AbstractPanel("Oscilloscope", false)
    , oscilloscopeComponent(fifoBuffer)
  {
    addAndMakeVisible(oscilloscopeComponent);
  }

  void resized() noexcept override
  {
    AbstractPanel::resized();
    oscilloscopeComponent.setBounds(getLocalBounds().reduced(30));
  }

private:
  OscilloscopeComponent oscilloscopeComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt