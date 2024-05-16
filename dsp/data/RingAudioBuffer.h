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
class RingAudioBuffer
{
  using AudioBuffer = juce::AudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

public:
  //============================================================================
  RingAudioBuffer(const int numChannelsToAllocate,
                  const int numSamplesToAllocate)
    : writePosition(0)
    , ringBuffer(numChannelsToAllocate, numSamplesToAllocate)
    , trackQueriedSamples(trackQueriedSamples)
    , ringBufferInterface(ringBuffer, *queriedSamples, writePosition)
  {
    if (trackQueriedSamples) {
      queriedSamples = std::make_unique<QueryList>(
        numChannelsToAllocate, std::vector<bool>(numSamplesToAllocate, true));
    }
  }
  //============================================================================
  void write(const AudioBuffer<SampleType>& bufferToWrite) noexcept
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

    if (trackQueriedSamples) {
      for (int channel = 0; channel < channelsToWrite; ++channel)
        for (int i = 0; i < samplesToWrite; ++i) {
          const int index = (writePosition + i) % getNumSamples();
          queriedSamples->at(channel).at(index) = false;
        }
    }

    writePosition = (writePosition + samplesToWrite) % getNumSamples();
  }
  void write(FifoAudioBuffer<SampleType>& bufferToWrite) noexcept
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
  inline AudioBuffer<SampleType>& getBuffer() noexcept { return ringBuffer; }
  //============================================================================
private:
  RingBufferInterface<SampleType> ringBufferInterface;
  AudioBuffer<SampleType> ringBuffer;
  int writePosition;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt