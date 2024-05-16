#pragma once
//==============================================================================
#include "dsp/data/RingAudioBuffer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class RingBufferInterface
{
  using AudioBuffer = juce::AudioBuffer<SampleType>;
  using QueryList = std::vector<std::vector<bool>>;
  using Span = std::span<SampleType>;

public:
  //============================================================================
  RingBufferInterface(AudioBuffer& audioBuffer,
                      QueryList& queryList,
                      const int& writePosition)
    : audioBuffer(audioBuffer)
    , queryList(queryList)
    , writePosition(writePosition)
  {
  }

protected:
  //============================================================================
  void setAudioSpan() noexcept {}
  //============================================================================
private:
  AudioBuffer& audioBuffer;
  Span audioSpan1;
  Span audioSpan2;
  QueryList& queryList;
  Span querySpan1;
  Span querySpan2;
  const int& writePosition;
  int lastWritePosition;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingBufferInterface)
};
} // namespace data
} // namespace dsp
} // namespace dmt