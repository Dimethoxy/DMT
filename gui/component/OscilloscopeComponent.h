#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingAudioBuffer.h"
#include "gui/widget/Graph.h"
#include "utility/RepaintTimer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
template<typename SampleType>
class OscilloscopeComponent
  : public juce::Component
  , public dmt::utility::RepaintTimer
{
  using Graph = dmt::gui::widget::Graph<SampleType>;
  using RingAudioBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

public:
  //==============================================================================
  OscilloscopeComponent(FifoAudioBuffer& fifoBuffer)
    : ringBuffer(2, 65536, true)
    , fifoBuffer(fifoBuffer)
    , leftGraph([this](int index) { return this->getLeftChannelSample(index); })
  {
    addAndMakeVisible(leftGraph);
    startRepaintTimer();
  }

  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    // TODO: Move painting the background to the OscilloscopePanel class
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());

    const int newestRaw = ringBuffer.getNewestUnqueriedIndexRaw();
    const int newest = ringBuffer.getNewestUnqueriedIndex();
    const int oldestRaw = ringBuffer.getOldestUnqueriedIndexRaw();
    const int oldest = ringBuffer.getOldestUnqueriedIndex();

    ringBuffer.write(fifoBuffer);
    leftGraph.repaint();
  }
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
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Graph leftGraph;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt