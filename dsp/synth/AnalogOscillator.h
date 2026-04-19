#pragma once

#include "dsp/synth/AnalogWaveform.h"
#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace synth {

class alignas(64) AnalogOscillator
{
public:
  struct Parameters
  {
    float drive = 0.0f;
    float bias = 0.0f;
  };

public:
  inline float getNextSample() noexcept
  {
    float sample = waveform.getSample(phase, delta);
    phase += delta;
    return sample;
  }

public:
  inline void setSampleRate(float _newSampleRate) noexcept
  {
    sampleRate = _newSampleRate;
    reset();
  }
  inline void setParameters(const juce::AudioProcessorValueTreeState& apvts,
                            String prefix)
  {
    String base = prefix + "AnalogOscillator";
    waveform.type = static_cast<AnalogWaveform::Type>(
      static_cast<int>(apvts.getRawParameterValue(base + "Type")->load()));
    params.drive = apvts.getRawParameterValue(base + "Drive")->load();
    params.bias = apvts.getRawParameterValue(base + "Bias")->load();
  }

  inline void setFrequency(float _newFrequency) noexcept
  {
    frequency = _newFrequency;
    delta = frequency / sampleRate;
  }

  void reset() noexcept
  {
    phase = 0.0f;
    delta = frequency / sampleRate;
  }

private:
  AnalogWaveform waveform;
  Parameters params;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float delta = 0.0f;
};
} // namespace synth
} // namespace dsp
} // namespace dmt