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
    : ringBuffer(2, 4096)
    , fifoBuffer(fifoBuffer)
    , leftOscilloscope(ringBuffer, 1)
    , rightOscilloscope(ringBuffer, 0)
  {
    // addAndMakeVisible(leftOscilloscope);
    addAndMakeVisible(rightOscilloscope);
    startRepaintTimer();
  }

  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    // leftOscilloscope.paint(g);
    rightOscilloscope.paint(g);
  }
  //==============================================================================
  void prepareToPaint() noexcept override
  {
    ringBuffer.write(fifoBuffer);
    leftOscilloscope.prepareToPaint();
    rightOscilloscope.prepareToPaint();
  }
  //==============================================================================
  void resized() override
  {
    auto bounds = getLocalBounds();
    auto halfHeight = bounds.getHeight() / 2;
    auto topBounds = bounds.removeFromTop(halfHeight);
    leftOscilloscope.setBounds(topBounds);
    auto lowerBounds = bounds.removeFromBottom(halfHeight);
    rightOscilloscope.setBounds(lowerBounds);
  }
  //==============================================================================
private:
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Oscilloscope leftOscilloscope;
  Oscilloscope rightOscilloscope;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt