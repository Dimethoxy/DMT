#pragma once

#include "dsp/filter/IIRFilter.h"
#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace effect {
class DisfluxProcessor
{
  using AudioBuffer = juce::AudioBuffer<float>;
  using ProcessSpec = juce::dsp::ProcessSpec;
  using Filter = dmt::dsp::filter::IIRFilterState;
  using FilterArray = std::array<Filter, 2>;
  using FilterMatrix = std::array<FilterArray, 2>;

public:
  DisfluxProcessor() { setCoefficents(400.0f, 1.0f); }

  void prepare(double newSampleRate)
  {
    this->sampleRate = (float)sampleRate;
    for (auto& channel : filters) {
      for (auto& filter : channel) {
        filter.sampleRate = sampleRate;
      }
    }
  }
  void processBlock(AudioBuffer& buffer)
  {
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      auto* channelData = buffer.getWritePointer(channel);
      auto channelFilters = filters[channel];
      for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float sample = channelData[i];
        for (auto& filter : channelFilters) {
          sample = filter.base.processSingleSampleRaw(sample);
        }
        channelData[i] = sample;
      }
    }
  }

protected:
  void setCoefficents(float frequency, float q)
  {
    const auto coefficients =
      juce::IIRCoefficients::makeHighPass(sampleRate, frequency, q);
    for (auto& channel : filters) {
      for (auto& filter : channel) {
        filter.base.setCoefficients(coefficients);
      }
    }
  }

private:
  float sampleRate = -1.0f;
  FilterMatrix filters;
};
} // namespace effect
} // namespace dsp
} // namespace dmt