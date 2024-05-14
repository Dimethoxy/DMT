//==============================================================================
#pragma once
//==============================================================================
#include <JuceHeader.h>
namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class SlicingRingAudioBuffer
{
public:
  //============================================================================
  SlicingAudioRingBuffer(const int numChannelsToAllocate,
                         const int numSamplesToAllocate)
    : bufferSize(numSamplesToAllocate)
    , numChannels(numChannelsToAllocate)
    , position(0)
    , buffer(numChannelsToAllocate, numSamplesToAllocate)
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
  // Overload [] operator to access the buffer in a read-only fashion
  const SampleType* operator[](const int channel) const noexcept
  {
    return ringBuffer.getReadPointer(channel);
  }
  //============================================================================
  const void resize(const int numChannelsToAllocate,
                    const int numSamplesToAllocate) noexcept
  {
    ringBuffer.setSize(numChannelsToAllocate, numSamplesToAllocate);
    return ringBuffer.getNumSamples();
  }
  //============================================================================
  inline const int getNumChannels() const noexcept
  {
    return ringBuffer.getNumChannels();
  }
  //============================================================================
  inline const int getNumSamples() const noexcept
  {
    return ringBuffer.getNumSamples();
  }
  //============================================================================
  inline const void setSliceSize(const int newSliceSize) noexcept
  {
    sliceSize = newSliceSize;
  }
  //============================================================================
  const void clear() noexcept
  {
    ringBuffer.clear();
    writePosition = 0;
  }
  //============================================================================
private:
  int writePosition;                  // Current starting position
  AudioBuffer<SampleType> ringBuffer; // Buffer to store audio data
  int sliceSize;                      // Size of the slice
};
} // namespace data
} // namespace dsp
} // namespace dmt