#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
public:
  DisfluxProcessor() {}

  ~DisfluxProcessor();

  void prepare(double newSampleRate)
  {
    sampleRate = newSampleRate;
    for (auto& filter : filters) {
      filter.prepare(sampleRate);
    }
  }
  void processBlock(juce::AudioBuffer<float>& buffer)
  {
    for (const auto& filter : filters) {
      processFilter(filter, buffer);
    }
  }

protected:
  void processFilter(juce::IIRFilter& filter, juce::AudioBuffer<float>& buffer)
  {
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      auto* channelData = buffer.getWritePointer(channel);
      for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        channelData[sample] = filter.processSample(channelData[sample]);
      }
    }
  }
  void setCoefficents(const juce::IIRCoefficients& newCoefficients)
  {
    const auto& coefficients =
      juce::IIRCoefficients::makeAllPass(sampleRate, 1000.0f);
    for (auto& filter : filters) {
      filter.setCoefficients(coefficients);
    }
  }

private:
  float sampleRate = -1.0f;
  std::array<juce::IIRFilter, 32> filters;
};
} // namespace effect
} // namespace dsp
} // namespace dmt