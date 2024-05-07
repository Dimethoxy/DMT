//==============================================================================
#pragma once
//==============================================================================
#include "dsp/data/AudioRingBuffer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace dsp {
namespace processor {
//==============================================================================
template<typename SampleType>
class OscilloscopeProcessor
{
  struct BufferBlock
  {
    std::vector<SampleType> buffer;
    bool rendered;
    juce::Image image;

    BufferBlock(numChannels, numSamples)
      : buffer(numChannels, std::vector<SampleType>(numSamples))
      , rendered(false)
    {
    }
  };

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
      // Wait until the buffer queue is not empty or the kill flag is set
      queueConditionVariable.wait(lock, [this] {
        return !this->bufferQueue.empty() || this->kill.get();
      });
      // Consumes the buffer queue
      while (!bufferQueue.empty()) {
        // Get the buffer from the queue
        const auto& buffer = bufferQueue.front();

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
private:
  // Blocks
  const int samplesPerBlock = 512;

  // Buffer queue
  std::queue<juce::AudioBuffer<SampleType>> bufferQueue;

  // Thread management
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