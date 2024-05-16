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
  using Spans = std::vector<std::span<SampleType>>;

public:
  //============================================================================
  RingBufferInterface(AudioBuffer& audioBuffer,
                      const int& writePosition,
                      std::vector<int>& readPosition)
    : audioBuffer(audioBuffer)
    , writePosition(writePosition)
    , readPosition(readPosition)
  {
  }
  //============================================================================
  SampleType getSample(const int channel, const int sample) const noexcept
  {
    const int numSamples = audioBuffer.getNumSamples();
    const int block1size = numSamples - writePosition;
    const int block2size = numSamples - block1size;
    if (sample < block1size) {
      return audioBuffer.getSample(channel, writePosition + sample);
    }
    return audioBuffer.getSample(channel, sample - block1size);
  }
  //============================================================================
  int getReadPosition(int channel) const noexcept
  {
    const int numSamples = audioBuffer.getNumSamples();
    const int rawReadPosition = readPosition[channel];
    if (rawReadPosition > writePosition) {
      return rawReadPosition - writePosition;
    }
    return numSamples - writePosition + rawReadPosition;
  }
  //============================================================================
  void incrementReadPosition(int channel, int increment) noexcept
  {
    const int numSamples = audioBuffer.getNumSamples();
    const int oldReadPosition = readPosition[channel];
    const int newReadPosition = (oldReadPosition + increment) % numSamples;
    readPosition[channel] = newReadPosition;
  }
  //============================================================================
private:
  AudioBuffer& audioBuffer;
  const int& writePosition;
  std::vector<int>& readPosition;

  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingBufferInterface)
};
} // namespace data
} // namespace dsp
} // namespace dmt