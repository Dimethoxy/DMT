#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingAudioBuffer.h"
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
  using RingAudioBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

public:
  //==============================================================================
  OscilloscopeComponent()
    : leftGraph([this](int index) { return this->getLeftChannelSample(index); })
    , ringBuffer(2, 2048)
  {
    addAndMakeVisible(leftGraph);
  }

  //==============================================================================
  void paint(juce::Graphics&) override {}
  void resized() override { leftGraph.setBounds(getLocalBounds()); }
  //==============================================================================
  SampleType getLeftChannelSample(int sampleIndex)
  {
    int channel = 0;
    int sliceSize = leftGraph.getWidth();
    return ringBuffer.getSampleFromNewestSlice(channel, sampleIndex, sliceSize);
  }
  //==============================================================================
private:
  Graph leftGraph;
  RingAudioBuffer ringBuffer;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt