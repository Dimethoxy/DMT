//==============================================================================
#pragma once
//==============================================================================
#include "dsp/data/RingBuffer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace dsp {
namespace processor {
//==============================================================================
template<typename SampleType>
class OscilloscopeProcessor
{
  using RingBuffer = dmt::dsp::data::RingBuffer<SampleType>;
  using BufferData = std::vector<std::vector<SampleType>>;
  using ChannelData = std::vector<SampleType>;

public:
  //============================================================================
  ~OscilloscopeProcessor() noexcept
  {
    // Set the kill flag to true to stop the transfer thread
    kill = true;
    // Wait until the transfer thread is closed
    std::unique_lock<std::mutex> lock(closeMutex);
    closeConditionVariable.wait(lock,
                                [this] { return this->closed.get() == true; });
    lock.unlock();
  }
  //============================================================================
  void prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/) noexcept
  {
    // Create the ring buffer
    ringBuffer = std::make_unique<RingBuffer>(numChannels, numSamples);
    // Start the transfer thread
    transferThread =
      std::thread(&OscilloscopeProcessor::transferThreadCallback, this);
    transferThread.detach();
  }
  //============================================================================
  void processBlock(juce::AudioBuffer<SampleType>& buffer) noexcept
  {
    juce::AudioBuffer<SampleType> bufferCopy(buffer);
    bufferQueue.push(bufferCopy);
    queueConditionVariable.notify_one();
  }
  //============================================================================
  void transferThreadCallback() noexcept
  {
    // Run this thread until the kill flag is set
    std::unique_lock<std::mutex> lock(queueMutex);
    while (!kill.get()) {
      // Persistent data
      BufferData bufferCache(numChannels, ChannelData(samplesPerPoint));
      int cacheIndex = 0;
      // Wait until the buffer queue is not empty or the kill flag is set
      queueConditionVariable.wait(lock, [this] {
        return !this->bufferQueue.empty() || this->kill.get();
      });
      // Consumes the buffer queue
      while (!bufferQueue.empty()) {
        // Get the buffer from the queue
        const auto& buffer = bufferQueue.front();
        const auto newNumSamples = buffer.getNumSamples();
        // Create a raw vector of audio data
        BufferData data(numChannels, ChannelData(newNumSamples));
        int bufferIndex = 0;
        // Copy the audio data from the buffer to the raw vector
        for (int sample = 0; sample < newNumSamples; ++sample) {
          for (int channel = 0; channel < numChannels; ++channel) {
            bufferCache[channel][cacheIndex] =
              buffer.getSample(channel, sample);
          }
          cacheIndex++;
          if (cacheIndex == samplesPerPoint) {
            auto points = extractPointsFromCache(bufferCache);
            for (int channel = 0; channel < numChannels; ++channel) {
              for (int point = 0; point < points[channel].size(); ++point) {
                data[channel][bufferIndex] = points[channel][point];
              }
            }
            cacheIndex = 0;
            bufferIndex++;
          }
        }
        // Resize all channels
        for (int channel = 0; channel < numChannels; ++channel) {
          data[channel].resize(bufferIndex);
        }
        // Write the audio data to the ring buffer
        ringBuffer->write(data);
        // Remove the consumed buffer from the queue
        bufferQueue.pop();
      }
    }
    // Gracefully close the thread and notify the destructor
    lock.unlock();
    closed = true;
    closeConditionVariable.notify_one();
  }
  const BufferData extractPointsFromCache(BufferData& cache) noexcept
  {
    BufferData result(numChannels, ChannelData(2));
    for (int channel = 0; channel < numChannels; ++channel) {
      auto max = std::max_element(cache[channel].begin(), cache[channel].end());
      auto maxIndex = std::distance(cache[channel].begin(), max);
      auto min = std::min_element(cache[channel].begin(), cache[channel].end());
      auto minIndex = std::distance(cache[channel].begin(), min);
      if (maxIndex < minIndex) {
        result[channel][0] = cache[channel][maxIndex];
        result[channel][1] = cache[channel][minIndex];
      } else {
        result[channel][0] = cache[channel][minIndex];
        result[channel][1] = cache[channel][maxIndex];
      }
    }
    return result;
  }

  //============================================================================
  const BufferData getAmplitudes(const int numDataPoints) noexcept
  {
    // Buffer with the original audio data
    auto buffer = ringBuffer->read();
    // Return the buffer with the downsampled audio data
    BufferData data(numChannels, ChannelData(numDataPoints));
    // Downsample the audio data using a lookup table
    // for (int channel = 0; channel < ringBufferNumChannels; ++channel) {
    //  // Create a lookup table for the audio data
    //  juce::dsp::LookupTable<SampleType> lookupTable(
    //    [buffer, channel](int x) { return buffer[channel][x]; },
    //    ringBufferNumSamples);
    //  // Downsample the audio data
    //  for (int dataPoint = 0; dataPoint < numDataPoints; ++dataPoint) {
    //    const auto normalizedIndex = (float)dataPoint / (float)numDataPoints;
    //    const auto index = normalizedIndex * (float)ringBufferNumSamples;
    //    const auto value = lookupTable.get(index);
    //    data[channel][dataPoint] = value;
    //  }
    //}
    // Return the downsampled audio data
    const auto offset = numSamples - numDataPoints;
    for (int channel = 0; channel < numChannels; ++channel) {
      for (int dataPoint = 0; dataPoint < numDataPoints; ++dataPoint) {
        data[channel][dataPoint] = buffer[channel][dataPoint + offset];
      }
    }
    return data;
  }

  //============================================================================
private:
  const int numChannels = 2;
  const int numSamples = 4096;
  const int samplesPerPoint = 16;
  std::shared_ptr<RingBuffer> ringBuffer;
  std::queue<juce::AudioBuffer<SampleType>> bufferQueue;
  std::condition_variable queueConditionVariable;
  std::condition_variable closeConditionVariable;
  std::mutex queueMutex;
  std::mutex closeMutex;
  std::thread transferThread;
  juce::Atomic<bool> kill{ false };
  juce::Atomic<bool> closed{ true };
};
} // namespace processor
} // namespace dsp
} // namespace dmt