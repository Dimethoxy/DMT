#pragma once
//==============================================================================
#include "dsp/processor/OscilloscopeProcessor.h"
#include "gui/widget/Curve.h"
#include "utility/RepaintTimer.h"
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
  OscilloscopeComponent(OscilloscopeProcessor& processor)
    : processor(processor)
    , curve([this](int numDataPoints) {
      return this->getLeftChannelAmplitudes(numDataPoints);
    })
  {
    addAndMakeVisible(curve);
  }

  void paint(juce::Graphics& g) override
  {
    g.fillAll(juce::Colours::green);
    const auto amplitudes = processor.getAmplitudes(100);
  }

  void resized() override { curve.setBounds(getLocalBounds()); }

  std::vector<SampleType> getLeftChannelAmplitudes(int numDataPoints)
  {
    return processor.getAmplitudes(numDataPoints)[0];
  }

private:
  OscilloscopeProcessor& processor;
  dmt::gui::widget::Curve<SampleType> curve;
};
} // namespace component
} // namespace gui
} // namespace dmt