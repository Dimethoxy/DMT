#pragma once
//==============================================================================
#include "dsp/data/AudioRingBuffer.h"
#include "gui/widget/Graph.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
template<typename SampleType>
class OscilloscopeComponent : public juce::Component
{
  using Graph = dmt::gui::widget::Graph<SampleType>;
  using AudioRingBuffer = dmt::dsp::data::AudioRingBuffer<SampleType>;

public:
  //==============================================================================
  OscilloscopeComponent()
    : graph([this](int numDataPoints) {
      return this->getLeftChannelAmplitudes(numDataPoints);
    })
    , audioRingBuffer(2, 65536)
  {
  }

  //==============================================================================
  void paint(juce::Graphics&) override {}
  void resized() override {}
  //==============================================================================
  const SampleType* getLeftChannelAmplitudes(int /*numDataPoints*/) {}
  //==============================================================================
  void fillDrawBuffer(int numDataPoints) { drawBuffer.clear(); }

private:
  Graph graph;
  std::vector<std::Vector<SampleType>> drawBuffer;
  AudioRingBuffer storageBuffer;
  juce::AudioBuffer<SampleType> drawBuffer;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt