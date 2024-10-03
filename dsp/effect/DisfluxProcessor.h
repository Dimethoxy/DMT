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
  using FilterArray = std::array<Filter, 1>;
  using FilterMatrix = std::array<FilterArray, 2>;

public:
  DisfluxProcessor() {}

  void prepare(double newSampleRate)
  {
    this->sampleRate = (float)newSampleRate;
    setCoefficents(400.0f, 1.0f);
  }
  void processBlock(AudioBuffer& buffer)
  {
    if (sampleRate <= 0.0f) {
      return;
    }

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      const auto left = buffer.getSample(0, sample);
      const auto right = buffer.getSample(1, sample);

      const auto leftFiltered = leftFilter.processSingleSampleRaw(left);
      const auto rightFiltered = rightFilter.processSingleSampleRaw(right);

      buffer.setSample(0, sample, leftFiltered);
      buffer.setSample(1, sample, rightFiltered);
    }
  }

protected:
  void setCoefficents(float frequency, float q)
  {
    const auto coefficients =
      juce::IIRCoefficients::makeLowPass((double)sampleRate, frequency, q);
    leftFilter.setCoefficients(coefficients);
    rightFilter.setCoefficients(coefficients);
  }

private:
  float sampleRate = -1.0f;
  juce::IIRFilter leftFilter;
  juce::IIRFilter rightFilter;
};
} // namespace effect
} // namespace dsp
} // namespace dmt