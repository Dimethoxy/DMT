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
    ringBuffer =
      std::make_unique<RingBuffer>(ringBufferNumChannels, ringBufferNumSamples);
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
      std::vector<SampleType> sampleSumCache(ringBufferNumChannels);
      int currentCacheSize = 0;
      int currentIndex = 0;
      // Wait until the buffer queue is not empty or the kill flag is set
      queueConditionVariable.wait(lock, [this] {
        return !this->bufferQueue.empty() || this->kill.get();
      });
      // Consumes the buffer queue
      while (!bufferQueue.empty()) {
        // Get the buffer from the queue
        const auto& buffer = bufferQueue.front();
        const auto numChannels = buffer.getNumChannels();
        const auto numSamples = buffer.getNumSamples();
        // Create a raw vector of audio data
        BufferData data(numChannels, ChannelData(numSamples));
        // Copy the audio data from the buffer to the raw vector
        for (int sample = 0; sample < numSamples; ++sample) {
          for (int channel = 0; channel < numChannels; ++channel) {
            sampleSumCache[channel] += buffer.getSample(channel, sample);
          }
          currentCacheSize++;
          if (currentCacheSize == samplesPerPoint) {
            for (int channel = 0; channel < numChannels; ++channel) {
              data[channel][currentIndex] =
                sampleSumCache[channel] / (SampleType)samplesPerPoint;
              sampleSumCache[channel] = 0;
            }
            currentIndex++;
            currentCacheSize = 0;
          }
        }
        // Resize all channels
        for (int channel = 0; channel < numChannels; ++channel) {
          data[channel].resize(currentIndex);
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

  //============================================================================
  const BufferData getAmplitudes(const int numDataPoints) noexcept
  {
    // Buffer with the original audio data
    auto buffer = ringBuffer->read();
    // Return the buffer with the downsampled audio data
    BufferData data(ringBufferNumChannels, ChannelData(numDataPoints));
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
    const auto offset = ringBufferNumSamples - numDataPoints;
    for (int channel = 0; channel < ringBufferNumChannels; ++channel) {
      for (int dataPoint = 0; dataPoint < numDataPoints; ++dataPoint) {
        data[channel][dataPoint] = buffer[channel][dataPoint + offset];
      }
    }
    return data;
  }

  //============================================================================
private:
  const int ringBufferNumChannels = 2;
  const int ringBufferNumSamples = 4096;
  const int samplesPerPoint = 32;
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