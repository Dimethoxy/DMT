#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingBufferInterface.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace dsp {
namespace data {
//==============================================================================
template<typename SampleType>
class RingAudioBuffer : public RingBufferInterface<SampleType>
{
  using AudioBuffer = juce::AudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;
  using RingBufferInterface = dmt::dsp::data::RingBufferInterface<SampleType>;

public:
  //============================================================================
  RingAudioBuffer(const int numChannelsToAllocate,
                  const int numSamplesToAllocate)
    : RingBufferInterface(ringBuffer, writePosition, readPositions)
    , writePosition(0)
    , ringBuffer(numChannelsToAllocate, numSamplesToAllocate)
    , readPositions(numChannelsToAllocate, 0)
  {
  }
  //============================================================================
  void write(const AudioBuffer& bufferToWrite) noexcept
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

    updateWritePosition(samplesToWrite);
  }
  //============================================================================
  void write(FifoAudioBuffer& bufferToWrite) noexcept
  {
    const int numChannels = getNumChannels();
    const int bufferSize = getNumSamples();
    const int channelsToWrite = bufferToWrite.getNumChannels();
    const int samplesToWrite = bufferToWrite.getNumReady();

    if (channelsToWrite > numChannels || samplesToWrite > bufferSize) {
      jassertfalse;
      return;
    }

    const AudioBuffer& source = bufferToWrite.getBuffer();
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

    updateWritePosition(samplesToWrite);
    bufferToWrite.finishedRead(size1 + size2);
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
  inline int getWritePosition() const noexcept { return writePosition; }
  //============================================================================
  void clear() noexcept
  {
    ringBuffer.clear();
    writePosition = 0;
  }
  //============================================================================
  inline AudioBuffer& getBuffer() noexcept { return ringBuffer; }
  //============================================================================

protected:
  //============================================================================
  void updateWritePosition(const int increment) noexcept
  {
    bool moveWriteOverRead = false;
    int newWritePosition = (writePosition + increment) % getNumSamples();
    if (writePosition == readPositions[0]) {
      writePosition = newWritePosition;
      return;
    }

    // Check if the new write position overlaps with any read positions
    for (int channel = 0; channel < getNumChannels(); ++channel) {
      for (int i = 0; i < increment; ++i) {
        if ((writePosition + i) % getNumSamples() == readPositions[channel]) {
          moveWriteOverRead = true;
          break;
        }
      }
    }
    // Update write position
    writePosition = newWritePosition;

    // Update read positions if necessary
    if (moveWriteOverRead) {
      for (int channel = 0; channel < getNumChannels(); ++channel) {
        readPositions[channel] = writePosition;
      }
    }
  }
  //============================================================================
private:
  AudioBuffer ringBuffer;
  int writePosition;
  std::vector<int> readPositions;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt