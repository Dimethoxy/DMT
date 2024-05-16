#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingAudioBuffer.h"
#include "gui/widget/Oscilloscope.h"
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
  using Oscilloscope = dmt::gui::widget::Oscilloscope<SampleType>;
  using RingAudioBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

public:
  //==============================================================================
  OscilloscopeComponent(FifoAudioBuffer& fifoBuffer)
    : ringBuffer(2, 100000)
    , fifoBuffer(fifoBuffer)
    , oscilloscope(ringBuffer, 0)
  {
    addAndMakeVisible(oscilloscope);
    startRepaintTimer();
  }

  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    // TODO: Move painting the background to the OscilloscopePanel class
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());
    /*
      const int newestRaw = ringBuffer.getNewestUnqueriedIndexRaw();
      const int newest = ringBuffer.getNewestUnqueriedIndex();
      const int oldestRaw = ringBuffer.getOldestUnqueriedIndexRaw();
      const int oldest = ringBuffer.getOldestUnqueriedIndex();
    */
    ringBuffer.write(fifoBuffer);
    oscilloscope.repaint();
  }
  void resized() override { oscilloscope.setBounds(getLocalBounds()); }
  //==============================================================================
private:
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Oscilloscope oscilloscope;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt