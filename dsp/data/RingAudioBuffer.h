#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include <JuceHeader.h>
//==============================================================================
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
    : writePosition(0)
    , ringBuffer(numChannelsToAllocate, numSamplesToAllocate)
  {
  }
  //============================================================================
  void write(const juce::AudioBuffer<SampleType>& bufferToWrite) noexcept
  {
    const int numChannels = getNumChannels();
    const int bufferSize = getNumSamples();
    const int channelsToWrite = bufferToWrite.getNumChannels();
    const int samplesToWrite = bufferToWrite.getNumSamples();

    if (channelsToWrite > numChannels || samplesToWrite > bufferSize) {
      jassertfalse;
      return;
    }

    const int firstBlockSize = getNumSamples() - writePosition;
    const int secondBlockSize = samplesToWrite - firstBlockSize;

    for (int channel = 0; channel < channelsToWrite; ++channel) {
      if (firstBlockSize > 0)
        ringBuffer.copyFrom(channel,         // destChannel
                            writePosition,   // destStartSample
                            bufferToWrite,   // source
                            channel,         // sourceChannel
                            0,               // sourceStartSample
                            firstBlockSize); // numSamples
      if (secondBlockSize > 0)
        ringBuffer.copyFrom(channel,          // destChannel
                            0,                // destStartSample
                            bufferToWrite,    // source
                            channel,          // sourceChannel
                            firstBlockSize,   // sourceStartSample
                            secondBlockSize); // numSamples
    }

    writePosition = (writePosition + samplesToWrite) % getNumSamples();
  }
  void write(
    dmt::dsp::data::FifoAudioBuffer<SampleType>& bufferToWrite) noexcept
  {
    const int numChannels = getNumChannels();
    const int bufferSize = getNumSamples();
    const int channelsToWrite = bufferToWrite.getNumChannels();
    const int samplesToWrite = bufferToWrite.getNumReady();

    if (channelsToWrite > numChannels || samplesToWrite > bufferSize) {
      jassertfalse;
      return;
    }

    const AudioBuffer<SampleType> source = bufferToWrite.getBuffer();
    int start1, size1, start2, size2;
    bufferToWrite.prepareToRead(samplesToWrite, start1, size1, start2, size2);

    // Block 1 Section 1
    if (size1 > 0) {
      int section1size = jmin(size1, bufferSize - writePosition);
      for (int channel = 0; channel < channelsToWrite; ++channel) {
        ringBuffer.copyFrom(channel,       // destChannel
                            writePosition, // destStartSample
                            source,        // source
                            channel,       // sourceChannel
                            start1,        // sourceStartSample
                            section1size); // numSamples
      }
      // Block 1 Section 2
      int section2size = size1 - section1size;
      if (section2size > 0) {
        for (int channel = 0; channel < channelsToWrite; ++channel) {
          ringBuffer.copyFrom(channel,               // destChannel
                              0,                     // destStartSample
                              source,                // source
                              channel,               // sourceChannel
                              start1 + section1size, // sourceStartSample
                              section2size);         // numSamples
        }
      }
    }
    // Block 2 Section 3
    if (size2 > 0) {
      // We need to start writing where we left off
      int block2start = (writePosition + size1) % bufferSize;
      int section3size = jmin(size2, bufferSize - block2start);
      for (int channel = 0; channel < channelsToWrite; ++channel) {
        ringBuffer.copyFrom(channel,       // destChannel
                            block2start,   // destStartSample
                            source,        // source
                            channel,       // sourceChannel
                            start2,        // sourceStartSample
                            section3size); // numSamples
      }
      // Block 2 Section 4
      int section4size = size2 - section3size;
      if (section4size > 0) {
        for (int channel = 0; channel < channelsToWrite; ++channel) {
          ringBuffer.copyFrom(channel,               // destChannel
                              0,                     // destStartSample
                              source,                // source
                              channel,               // sourceChannel
                              start2 + section3size, // sourceStartSample
                              section4size);         // numSamples
        }
      }
    }
    writePosition = (writePosition + samplesToWrite) % getNumSamples();
    bufferToWrite.finishedRead(size1 + size2);
  }
  //============================================================================
  inline SampleType getSample(const int channel,
                              const int sample) const noexcept
  {
    if (sample < 0 || sample >= getNumSamples()) {
      jassertfalse;
      return SampleType(0);
    }
    const int readPosition = (writePosition + sample) % getNumSamples();
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
    const int numSamples = getNumSamples();
    const int sliceStart =
      (writePosition - sliceSize + numSamples) % numSamples;
    const int readPosition = (sliceStart + sample) % numSamples;
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
  void clear() noexcept
  {
    ringBuffer.clear();
    writePosition = 0;
  }
  //============================================================================
private:
  int writePosition;                  // Current starting position
  AudioBuffer<SampleType> ringBuffer; // Buffer to store audio data
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt