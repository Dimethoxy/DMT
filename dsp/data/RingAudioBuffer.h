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
  using QueryList = std::array<bool>;
  using QueryListPtr = std::unique_ptr<QueryList>;

  //============================================================================
  RingAudioBuffer(const int numChannelsToAllocate,
                  const int numSamplesToAllocate,
                  bool trackQueriedSamples = false)
    : writePosition(0)
    , ringBuffer(numChannelsToAllocate, numSamplesToAllocate)
  {
    if (trackQueriedSamples) {
      queriedSamples = std::make_unique<QueryList>(numSamplesToAllocate);
      queriedSamples->fill(false);
    }
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

    if (trackQueriedSamples) {
      for (int i = 0; i < samplesToWrite; ++i) {
        const index = (writePosition + i) % getNumSamples();
        queriedSamples.get()[index] = false;
      }
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

    if (trackQueriedSamples) {
      for (int i = 0; i < samplesToWrite; ++i) {
        const index = (writePosition + i) % getNumSamples();
        queriedSamples.get()[index] = false;
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
    if (trackQueriedSamples) {
      queriedSamples.get()[readPosition] = true;
    }
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
    if (trackQueriedSamples) {
      queriedSamples.get()[readPosition] = true;
    }
    return ringBuffer.getSample(channel, readPosition);
  }
  //============================================================================
  int getOldestUnqueriedIndexRaw() const noexcept
  {
    if (!trackQueriedSamples) {
      jassertfalse;
      return -1;
    }
    const int numSamples = getNumSamples();
    for (int i = 0; i < numSamples; ++i) {
      if (!queriedSamples.get()[i]) {
        return i;
      }
    }
    return -1;
  }
  //============================================================================
  int getOldestUnqueriedIndex() const noexcept
  {
    const rawIndex = getOldestUnqueriedIndexRaw();
    if (rawIndex < 0) {
      return -1;
    }
    return mapRawIndex(rawIndex);
  }
  //============================================================================
  int getNewestUnqueriedIndexRaw() const noexcept
  {
    if (!trackQueriedSamples) {
      jassertfalse;
      return -1;
    }
    const int numSamples = getNumSamples();
    for (int i = numSamples - 1; i >= 0; --i) {
      if (!queriedSamples.get()[i]) {
        return i;
      }
    }
    return -1;
  }
  //============================================================================
  int getNewestUnqueriedIndex() const noexcept
  {
    const rawIndex = getNewestUnqueriedIndexRaw();
    if (rawIndex < 0) {
      return -1;
    }
    return mapRawIndex(rawIndex);
  }
  //============================================================================
  int mapRawIndex(const int rawIndex) const noexcept
  {
    if (rawIndex < 0) {
      return -1;
    }
    const int numSamples = getNumSamples();
    return (writePosition + rawIndex) % numSamples;
  }
  //============================================================================
  void resize(const int numChannelsToAllocate,
              const int numSamplesToAllocate) noexcept
  {
    ringBuffer.setSize(numChannelsToAllocate, numSamplesToAllocate);
    if (trackQueriedSamples) {
      queriedSamples = std::make_unique<QueryList>(numSamplesToAllocate);
      queriedSamples->fill(false);
    }
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
    if (trackQueriedSamples) {
      queriedSamples->fill(false);
    }
  }
  //============================================================================
private:
  int writePosition;
  AudioBuffer<SampleType> ringBuffer;
  QueryListPtr queriedSamples;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingAudioBuffer)
};
} // namespace data
} // namespace dsp
} // namespace dmt