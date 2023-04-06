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
    auto bendedPhase = phase;

    float positiveCycleSize = posityCycleRatio * twoPi;
    float negativeCycleRatio = 1.0f - posityCycleRatio;
    float negativeCycleSize = negativeCycleRatio * twoPi;

    if (phase <= positiveCycleSize) {
      bendedPhase /= (posityCycleRatio * 2.0f);
    }

    if (phase > positiveCycleSize) {
      bendedPhase = (phase - positiveCycleSize) / negativeCycleSize;
      bendedPhase = bendedPhase * pi + pi;
    }

    auto samplePhase = bendedPhase * pwmAmount;

    if (samplePhase >= twoPi)
      return 0.0f;
    else
      return waveform.getSample(samplePhase * syncAmount);
  }
  //============================================================================
  void setFrequency(float frequency) noexcept { this->frequency = frequency; }
  void setPhase(float newPhase) { this->phase = newPhase; }
  void setWaveformType(dmt::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }
  void setSyncAmount(float syncAmount)
  {
    if (syncAmount < 1.0f)
      jassert("Can't set syncAmount smaller then 1.0");
    this->syncAmount = syncAmount;
  }
  void setPwmAmount(float pwmAmount)
  {
    if (pwmAmount < 1.0f)
      jassert("Can't set pwmAmount smaller then 1.0");
    this->pwmAmount = pwmAmount;
  }
  void setPosityCycleRatio(float posityCycleRatio)
  {
    if (posityCycleRatio <= 0.0f)
      jassert("Can't set posityCycleRatio smaller or equal then 0.0");
    if (posityCycleRatio >= 1.0f)
      jassert("Can't set posityCycleRatio greater or equal then 1.0");
    this->posityCycleRatio = posityCycleRatio;
  }
  //============================================================================
private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float pwmAmount = 1.0f;
  float syncAmount = 1.0f;
  float posityCycleRatio = 0.5f;
};
}
//==============================================================================
