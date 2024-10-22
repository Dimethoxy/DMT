#pragma once

#include <JuceHeader.h>
#include <utility/Settings.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
  using AudioBuffer = juce::AudioBuffer<float>;
  using ProcessSpec = juce::dsp::ProcessSpec;
  using Filter = juce::IIRFilter;
  using FilterArray = std::array<Filter, 100>;
  const float& tolerance = dmt::Settings::floatTolerance;

public:
  DisfluxProcessor(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
  {
  }

  void prepare(double newSampleRate)
  {
    this->sampleRate = (float)newSampleRate;
    setCoefficents();
  }
  void processBlock(AudioBuffer& buffer)
  {
    if (sampleRate <= 0.0f) {
      return;
    }

    const auto newFrequency =
      apvts.getRawParameterValue("DisfluxFrequency")->load();
    const auto newPinch = apvts.getRawParameterValue("DisfluxPinch")->load();
    const auto amount = apvts.getRawParameterValue("DisfluxAmount")->load();

    if (std::abs(newFrequency - frequency) > tolerance ||
        std::abs(newPinch - pinch) > tolerance) {
      frequency = newFrequency;
      pinch = newPinch;
      setCoefficents();
    }

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      auto left = buffer.getSample(0, sample);
      auto right = buffer.getSample(1, sample);

      for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
        left = leftFilters[filterIndex].processSingleSampleRaw(left);
        right = rightFilters[filterIndex].processSingleSampleRaw(right);
      }

      buffer.setSample(0, sample, left);
      buffer.setSample(1, sample, right);
    }
  }

protected:
  void setCoefficents()
  {
    const auto coefficients =
      juce::IIRCoefficients::makeAllPass((double)sampleRate, frequency, pinch);

    for (auto& filter : leftFilters) {
      filter.setCoefficients(coefficients);
    }
    for (auto& filter : rightFilters) {
      filter.setCoefficients(coefficients);
    }
  }

private:
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;
  float frequency = 800.0f;
  float pinch = 1.0f;
  FilterArray leftFilters;
  FilterArray rightFilters;
};
} // namespace effect
} // namespace dsp
} // namespace dmt