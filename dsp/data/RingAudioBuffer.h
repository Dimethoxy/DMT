//==============================================================================
#pragma once
//==============================================================================
#include <JuceHeader.h>
namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class RingAudioBuffer
{
public:
  //============================================================================
  RingAudioBuffer(const int numChannelsToAllocate,
                  const int numSamplesToAllocate)
    : bufferSize(numSamplesToAllocate)
    , numChannels(numChannelsToAllocate)
    , writePosition(0)
    , ringBuffer(numChannelsToAllocate, numSamplesToAllocate)
  {
  }
  //============================================================================
  void write(const juce::AudioBuffer<SampleType>& bufferToWrite) noexcept
  {
    samplesToWrite = bufferToWrite.getNumSamples();
    channelsToWrite = bufferToWrite.getNumChannels();

    if (channelsToWrite > numChannels || samplesToWrite > bufferSize) {
      jassertfalse;
      return;
    }

    const int firstBlockSize = getNumSamples() - writePosition;
    const int secondBlockSize = samplesToWrite - firstBlockSize;

    for (int channel = 0; channel < channelsToWrite; ++channel) {
      ringBuffer.copyFrom(channel,          // destChannel
                          writePosition,    // destStartSample
                          bufferToWrite,    // source
                          channel,          // sourceChannel
                          0,                // sourceStartSample
                          firstBlockSize);  // numSamples
      ringBuffer.copyFrom(channel,          // destChannel
                          0,                // destStartSample
                          bufferToWrite,    // source
                          channel,          // sourceChannel
                          firstBlockSize,   // sourceStartSample
                          secondBlockSize); // numSamples
    }

    writePosition = (writePosition + samplesToWrite) % getNumSamples();
  }
  //============================================================================
  inline SampleType getSample(const int channel,
                              const int sample) const noexcept
  {
    if (sample < 0 || sample >= getNumSamples()) {
      jassertfalse;
      return SampleType(0);
    }
    const int readPosition = (position + sample) % getNumSamples();
    return ringBuffer.getSample(channel, readPosition);
  }
  //============================================================================
  inline SampleType getSampleFromNewestSlice(const int channel,
                                             const int sample,
                                             const int sliceSize) const noexcept
  {
    if (sample < 0 || sample >= sliceSize) {
      jassertfalse;
      return SampleType(0);
    }
    const int sliceStart = (writePosition - sliceSize) % getNumSamples();
    const int readPosition = (sliceStart + sample) % getNumSamples();
    return ringBuffer.getSample(channel, readPosition);
  }
  //============================================================================
  void resize(const int numChannelsToAllocate,
              const int numSamplesToAllocate) noexcept
  {
    ringBuffer.setSize(numChannelsToAllocate, numSamplesToAllocate);
  }
  //============================================================================
  inline int getNumChannels() const noexcept
  {
    return ringBuffer.getNumChannels();
  }
  //============================================================================
  inline int getNumSamples() const noexcept
  {
    return ringBuffer.getNumSamples();
  }
  //============================================================================
  inline void setSliceSize(const int newSliceSize) noexcept
  {
    sliceSize = newSliceSize;
  }
  //============================================================================
  void clear() noexcept
  {
    ringBuffer.clear();
    writePosition = 0;
  }
  //============================================================================
private:
  int writePosition;                  // Current starting position
  AudioBuffer<SampleType> ringBuffer; // Buffer to store audio data
};
} // namespace data
} // namespace dsp
} // namespace dmt