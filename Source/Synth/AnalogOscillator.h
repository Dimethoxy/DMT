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
  /**
   * Calculates and returns the next sample of the oscillator waveform.
   *
   * If the sample rate has not been set or is less than or equal to zero,
   * the function returns 0.0f.
   *
   * @return                The next sample of the oscillator waveform.
   */
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

    auto samplePhase = bendedPhase * pwmModifier;

    if (samplePhase >= twoPi)
      return 0.0f;
    else
      return waveform.getSample(samplePhase * syncAmount);
  }

  //============================================================================
  /**
   * Sets the frequency of the oscillator waveform.
   *
   * @param frequency       The frequency value in Hz. A JUCE assertion is
   *                        triggered if the frequency is outside the allowed
   *                        range.
   */
  void setFrequency(const float frequency) noexcept
  {
    const juce::Range<float> frequencyRange(20.0f, 20000.0f);
    jassert(frequencyRange.contains(frequency));
    this->frequency = frequency;
  }

  //============================================================================
  /**
   * Sets the phase of the oscillator waveform.
   *
   * @param newPhase        The phase value in radians.
   */
  void setPhase(const float newPhase) noexcept { this->phase = newPhase; }

  //============================================================================
  /**
   * Sets the type of the oscillator waveform.
   *
   * @param type            The waveform type to set.
   */
  void setWaveformType(const dmt::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }

  //============================================================================
  /**
   * Sets the amount of oscillator sync for the oscillator waveform.
   *
   * @param syncModifier    The sync modifier value in the range of 0.0f to
   *                        100.0f. The value specifies the percentage of the
   *                        max amount of 5 syncs. A JUCE assertion is triggered
   *                        if the value is outside the allowed range.
   */
  void setSyncAmount(const float syncModifier)
  {
    const juce::NormalisableRange<float> sourceRange(0.0f, 100.0f);
    jassert(sourceRange.getRange().contains(syncAmount));
    const auto normalisedValue = sourceRange.convertTo0to1(syncAmount);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->syncAmount = targetRange.convertFrom0to1(normalisedValue);
  }

  //============================================================================
  /**
   * Sets the amount of pulse-width modulation (PWM) for the oscillator
   * waveform.
   *
   * @param pwmModifier     The PWM modifier value in the range of 0.0f to
   *                        100.0f. The value specifies the percentage of time
   *                        that the waveform spends in its positive cycle.
   *                        A JUCE assertion is triggered if the value is
   *                        outside the allowed range.
   */
  void setPwmAmount(const float pwmModifier) noexcept
  {
    const juce::NormalisableRange<float> sourceRange(0.0f, 100.0f);
    jassert(sourceRange.getRange().contains(pwmModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(pwmModifier);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->pwmModifier = targetRange.convertFrom0to1(normalisedValue);
  }

  //============================================================================
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
  void setBend(const float bendModifier) noexcept
  {
    const juce::NormalisableRange<float> sourceRange(-100.0f, 100.0f);
    jassert(sourceRange.getRange().contains(bendModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(bendModifier);
    const juce::NormalisableRange<float> targetRange(0.1f, 0.9f);
    this->posityCycleRatio = targetRange.convertFrom0to1(normalisedValue);
  }
  //============================================================================
private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float pwmModifier = 1.0f;
  float syncAmount = 1.0f;
  float posityCycleRatio = 0.5f;
};
}
//==============================================================================
