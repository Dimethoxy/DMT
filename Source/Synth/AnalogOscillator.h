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
  /**
   * Sets the bend modifier for the oscillator waveform.
   *
   * @param bendModifier    The bend modifier value in the range of -100.0f to
   *                        100.0f. A negative value reduces the positive cycle
   *                        length, while a positive value reduces the negative
   *                        cycle length. A value of zero leaves the waveform
   *                        unmodified. A JUCE assertion is triggered if the
   *                        value is outside the allowed range.
   */
  void setBend(float bendModifier)
  {
    // Define the valid range for the bend modifier
    const juce::NormalisableRange<float> modifierRange(-100.0f, 100.0f);

    // Check if the value is within the valid range
    jassert(modifierRange.getRange().contains(bendModifier));

    // Convert the bend modifier to a value between 0.1 and 0.9,
    // which represents the ratio of positive cycle length to the total cycle
    // length
    const auto normalisedValue = modifierRange.convertTo0to1(bendModifier);

    // Define the range of valid cycle ratios
    const juce::NormalisableRange<float> cycleRange(0.1f, 0.9f);

    // Convert the normalised value to a cycle ratio in the valid range
    this->posityCycleRatio = cycleRange.convertFrom0to1(normalisedValue);
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
