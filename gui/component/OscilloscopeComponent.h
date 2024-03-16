#pragma once
//==============================================================================
#include "../../dsp/processor/OscilloscopeProcessor.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
template<typename SampleType>
class OscilloscopeComponent
  : public juce::Component
  , public juce::Timer
{
  using OscilloscopeProcessor =
    dmt::dsp::processor::OscilloscopeProcessor<SampleType>;

public:
  OscilloscopeComponent(OscilloscopeProcessor& processor)
    : processor(processor)
  {
    startTimerHz(60);
  }

  void paint(juce::Graphics& g) override
  {
    g.fillAll(juce::Colours::green);
    const auto amplitudes = processor.getAmplitudes(100);
  }

  void resized() override {}

  void timerCallback() override { repaint(); }

private:
  OscilloscopeProcessor& processor;
};
} // namespace component
} // namespace gui
} // namespace dmt