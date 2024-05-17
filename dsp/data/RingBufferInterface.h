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
  void equalizeReadPositions() noexcept
  {
    void equalizeReadPositions() noexcept
    {
      int highestReadPosition = 0;
      int highestReadChannel = 0;

      // Find the highest read position among all channels
      for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
        const int readPos = getReadPosition(channel);
        if (readPos > highestReadPosition) {
          highestReadPosition = readPos;
          highestReadChannel = channel;
        }
      }

      // Get the raw read position for the channel with the highest read
      // position
      const int highestRawReadPosition =
        audioBuffer.getRawReadPosition(highestReadChannel);

      // Set all channels to the highest raw read position found
      for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
        audioBuffer.setRawReadPosition(channel, highestRawReadPosition);
      }
    }
  }

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