#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
  using ProcessSpec = juce::dsp::ProcessSpec;

public:
  DisfluxProcessor() {}

  void prepare(double newSampleRate)
  {
    sampleRate = newSampleRate;
    auto maximumBlockSize = static_cast<juce::uint32>(4096);
    auto processSpec = ProcessSpec{ sampleRate, maximumBlockSize, 2 };
    for (auto& filter : filters) {
      filter.prepare(processSpec);
    }
  }
  void processBlock(juce::AudioBuffer<float>& buffer)
  {
    for (auto& filter : filters) {
      processFilter(filter, buffer);
    }
  }

protected:
  void processFilter(juce::dsp::IIR::Filter<float>& filter,
                     juce::AudioBuffer<float>& buffer)
  {
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      auto* channelData = buffer.getWritePointer(channel);
      for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        channelData[sample] = filter.processSample(channelData[sample]);
      }
    }
  }

private:
  float sampleRate = -1.0f;
  std::array<juce::dsp::IIR::Filter<float>, 32> filters;
};
} // namespace effect
} // namespace dsp
} // namespace dmt