#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
  using AudioBuffer = juce::AudioBuffer<float>;
  using ProcessSpec = juce::dsp::ProcessSpec;
  using Filter = juce::IIRFilter;
  using FilterArray = std::array<Filter, 32>;

public:
  DisfluxProcessor(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
  {
  }

  void prepare(double newSampleRate)
  {
    this->sampleRate = (float)newSampleRate;
    setCoefficents(800.0f, 1.0f);
  }
  void processBlock(AudioBuffer& buffer)
  {
    if (sampleRate <= 0.0f) {
      return;
    }

    const auto newFrequency =
      apvts.getRawParameterValue("DisfluxFrequency")->load();

    if (newFrequency != frequency) {
      frequency = newFrequency;
      setCoefficents(frequency, 1.0f);
    }

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      auto left = buffer.getSample(0, sample);
      auto right = buffer.getSample(1, sample);

      for (auto& filter : leftFilters) {
        left = filter.processSingleSampleRaw(left);
      }
      for (auto& filter : rightFilter) {
        right = filter.processSingleSampleRaw(right);
      }

      buffer.setSample(0, sample, left);
      buffer.setSample(1, sample, right);
    }
  }

protected:
  void setCoefficents(float frequency, float q)
  {
    const auto coefficients =
      juce::IIRCoefficients::makeAllPass((double)sampleRate, frequency, q);

    for (auto& filter : leftFilters) {
      filter.setCoefficients(coefficients);
    }
    for (auto& filter : rightFilter) {
      filter.setCoefficients(coefficients);
    }
  }

private:
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;
  float frequency = 0.0f;
  FilterArray leftFilters;
  FilterArray rightFilter;
};
} // namespace effect
} // namespace dsp
} // namespace dmt