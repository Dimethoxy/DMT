#pragma once

#include <JuceHeader.h>
#include <utility/Settings.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
  constexpr static int FILTER_AMOUNT = 100;
  using AudioBuffer = juce::AudioBuffer<float>;
  using ProcessSpec = juce::dsp::ProcessSpec;
  using Filter = juce::IIRFilter;
  using FilterArray = std::array<Filter, FILTER_AMOUNT>;
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

    const int newAmount = apvts.getRawParameterValue("DisfluxAmount")->load();
    const int newSpread = apvts.getRawParameterValue("DisfluxSpread")->load();
    const auto newFrequency =
      apvts.getRawParameterValue("DisfluxFrequency")->load();
    const auto newPinch = apvts.getRawParameterValue("DisfluxPinch")->load();

    if (amount != newAmount || spread != newSpread ||
        std::abs(frequency - newFrequency) > tolerance ||
        std::abs(pinch - newPinch) > tolerance) {
      amount = newAmount;
      spread = newSpread;
      frequency = newFrequency;
      pinch = newPinch;
      setCoefficents();
    }

    for (size_t sample = 0; sample < buffer.getNumSamples(); ++sample) {
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
    const float spreadAmount = (float)spread;
    const float maxFrequency = 20000.0f;
    const float rangeStartFrequency =
      std::clamp(frequency - (spreadAmount / 2.0f), 20.0f, 20000.0f);
    const float rangeEndFrequency =
      std::clamp(frequency + (spreadAmount / 2.0f), 20.0f, 20000.0f);

    const float logStartFrequency = std::log(rangeStartFrequency);
    const float logEndFrequency = std::log(rangeEndFrequency);
    const float logFrequencyDelta = logEndFrequency - logStartFrequency;

    for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
      const float logFrequencyOffsetFactor =
        (amount == 1) ? 0.5f : (float)filterIndex / (amount - 1);

      const float logFrequency = std::exp(
        logStartFrequency + (logFrequencyDelta * logFrequencyOffsetFactor));

      const auto coefficients = juce::IIRCoefficients::makeAllPass(
        (double)sampleRate, logFrequency, pinch);

      leftFilters[filterIndex].setCoefficients(coefficients);
      rightFilters[filterIndex].setCoefficients(coefficients);
    }
  }

private:
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;
  int amount = 1;
  int spread = 0;
  float frequency = 800.0f;
  float pinch = 1.0f;
  FilterArray leftFilters;
  FilterArray rightFilters;
};
} // namespace effect
} // namespace dsp
} // namespace dmt