//==============================================================================

#pragma once

#include "AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class AnalogOscillator
{
  const float twoPi = juce::MathConstants<float>::twoPi;
  const float pi = juce::MathConstants<float>::pi;

public:
  //============================================================================
  void setSampleRate(float sampleRate) noexcept
  {
    this->sampleRate = sampleRate;
  }
  //============================================================================
  float getNextSample() noexcept
  {
    if (sampleRate <= 0.0f)
      return 0.0f;

    float cycleLength = sampleRate / frequency;
    float phaseDelta = twoPi / cycleLength;
    phase += phaseDelta;

    if (phase >= twoPi) {
      phase -= twoPi;
    }

    auto samplePhase = std::pow((phase*pwmFactor), bendFactor);
    if (samplePhase >= twoPi)
      return 0.0f;
    else
      return waveform.getSample(samplePhase);
  }
  //============================================================================
  void setFrequency(float frequency) noexcept { this->frequency = frequency; }
  void setPhase(float newPhase) { this->phase = newPhase; }
  void setWaveformType(dmt::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }
  //============================================================================
private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float dutyCycle = 0.5f;
  float pwmFactor = 1.0f;
  float bendFactor = 1.0f;
};
}
//==============================================================================
