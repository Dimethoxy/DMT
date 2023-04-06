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
      auto normalizedPhase = phase - negativeCycleSize;
      auto relativePhasePosition = normalizedPhase / negativeCycleSize;
    }

    auto samplePhase = bendedPhase * pwmFactor;

    if (samplePhase >= twoPi)
      return 0.0f;
    else
      return waveform.getSample(samplePhase * syncFactor);
  }
  //============================================================================
  void setFrequency(float frequency) noexcept { this->frequency = frequency; }
  void setPhase(float newPhase) { this->phase = newPhase; }
  void setWaveformType(dmt::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }
  void setSyncFactor(float syncFactor)
  {
    if (syncFactor < 1.0f)
      jassert("Can't set syncFactor smaller then 1.0");
    this->syncFactor = syncFactor;
  }
  // void setBendFactor(float linearValue)
  //{
  //   this->bendExponent = dmt::Math::linearToExponent(linearValue);
  // }
  void setPwmFactor(float pwmFactor)
  {
    if (pwmFactor < 1.0f)
      jassert("Can't set pwmFactor smaller then 1.0");
    this->pwmFactor = pwmFactor;
  }
  //============================================================================
private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float pwmFactor = 1.0f;
  float syncFactor = 1.0f;
  float posityCycleRatio = 0.25;
};
}
//==============================================================================
