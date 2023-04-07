#pragma once

#include "AnalogWaveform.h"
#include <JuceHeader.h>

namespace dmt {
class AnalogOscillator
{
  inline static const float twoPi = juce::MathConstants<float>::twoPi;
  inline static const float pi = juce::MathConstants<float>::pi;

public:
  inline void setSampleRate(const float sampleRate) noexcept
  {
    float rangeEnd =
      std::nextafter(392000.0f, std::numeric_limits<float>::max());
    const juce::Range<float> validRange(20.0f, rangeEnd);
    jassert(validRange.contains(sampleRate));
    this->sampleRate = sampleRate;
  }

  inline float getNextSample() noexcept
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
      return waveform.getSample(samplePhase * syncModifier);
  }

  inline void setFrequency(const float frequency) noexcept
  {
    const juce::NormalisableRange<float> frequencyRange(20.0f, 20000.1f);
    jassert(frequencyRange.getRange().contains(frequency));
    this->frequency = frequency;
  }

  inline void setPhase(const float newPhase) noexcept
  {
    this->phase = newPhase;
  }

  inline void setWaveformType(const dmt::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }

  inline void setSync(const float syncModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(syncModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(syncModifier);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->syncModifier = targetRange.convertFrom0to1(normalisedValue);
  }

  inline void setPwm(const float pwmModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(pwmModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(pwmModifier);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->pwmModifier = targetRange.convertFrom0to1(normalisedValue);
  }

  inline void setBend(const float bendModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(-100.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(bendModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(bendModifier);
    const juce::NormalisableRange<float> targetRange(0.1f, 0.9f);
    this->posityCycleRatio = targetRange.convertFrom0to1(normalisedValue);
  }

private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  float pwmModifier = 1.0f;
  float syncModifier = 1.0f;
  float posityCycleRatio = 0.5f;
};
}
