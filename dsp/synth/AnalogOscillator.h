#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace synth {

class alignas(64) AnalogOscillator
{
public:
  struct Parameters
  {
    // AnalogWaveform::Type type = AnalogWaveform::Type::Sine;
    float drive = 0.0f;
    float bias = 0.0f;
  };

public:
  inline float getNextSample() noexcept
  {
    float sample = waveform.getSample(phase);
    phase += waveform.delta;
    if (phase >= 1.0f)
      phase -= 1.0f;
    return sample;
  }

public:
  inline void setSampleRate(float _newSampleRate) noexcept
  {
    sampleRate = _newSampleRate;
    updateDelta();
  }
  inline void setParameters(const juce::AudioProcessorValueTreeState& apvts,
                            String prefix)
  {
    String base = prefix + "AnalogOscillator";
    params.drive = apvts.getRawParameterValue(base + "Drive")->load();
    params.bias = apvts.getRawParameterValue(base + "Bias")->load();
  }

  inline void setFrequency(float _newFrequency) noexcept
  {
    frequency = _newFrequency;
    updateDelta();
  }

  void reset() noexcept
  {
    phase = 0.0f;
    waveform.lastPhase = 0.0f;
  }

private:
  inline void updateDelta() noexcept
  {
    if (sampleRate > 0.0f)
      waveform.delta = frequency / sampleRate;
  }

private:
  AnalogWaveform waveform;
  Parameters params;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
};
} // namespace synth
} // namespace dsp
} // namespace dmt