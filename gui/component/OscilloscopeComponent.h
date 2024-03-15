#pragma once
//==============================================================================
#include "../../dsp/processor/OscilloscopeProcessor.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
template<typename SampleType>
class OscilloscopeComponent : public juce::Component
{
  using OscilloscopeProcessor =
    dmt::dsp::processor::OscilloscopeProcessor<SampleType>;

public:
  OscilloscopeComponent(OscilloscopeProcessor& processor) {}

  void paint(juce::Graphics& g) override {}

  void resized() override {}
};
} // namespace component
} // namespace gui
} // namespace dmt