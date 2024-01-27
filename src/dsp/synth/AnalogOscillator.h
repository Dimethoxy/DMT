#pragma once

#include "AnalogWaveform.h"
#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace synth {
class AnalogOscillator
{
  using Math = juce::dsp::FastMathApproximations;
  inline static const float twoPi = juce::MathConstants<float>::twoPi;
  inline static const float pi = juce::MathConstants<float>::pi;

public:
  inline void setSampleRate(const float newSampleRate) noexcept
  {
    float rangeEnd =
      std::nextafter(392000.0f, std::numeric_limits<float>::max());
    const juce::Range<float> validRange(20.0f, rangeEnd);
    jassert(validRange.contains(newSampleRate));
    this->sampleRate = newSampleRate;
  }

  inline float getNextSample() noexcept
  {
    if (sampleRate <= 0.0f)
      return 0.0f;

    advancePhase();

    auto syncedPhase = getSyncedPhase(phase * pwmModifier);
    auto bendedPhase = getBendedPhase(syncedPhase);

    if (phase >= twoPi / pwmModifier)
      return 0.0f;

    float sample = waveform.getSample(bendedPhase);
    distortSample(sample);
    return std::clamp(sample, -1.0f, +1.0f);
  }

  inline void setFrequency(const float newFequency) noexcept
  {
    this->frequency = newFequency;
  }

  inline void setWaveformType(
    const dmt::dsp::synth::AnalogWaveform::Type type) noexcept
  {
    waveform.type = type;
  }

  inline void setDrive(const float newDrive) noexcept
  {
    this->drive = newDrive;
  }

  inline void setBias(const float newBias) noexcept { this->bias = newBias; }

  inline void setPhase(const float newPhase) noexcept
  {
    this->phase = newPhase;
  }

  inline void setBend(const float newBendModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(-100.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(newBendModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(newBendModifier);
    const juce::NormalisableRange<float> targetRange(0.1f, 0.9f);
    this->posityCycleRatio = targetRange.convertFrom0to1(normalisedValue);
  }

  inline void setPwm(const float newPwmModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(newPwmModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(newPwmModifier);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->pwmModifier = targetRange.convertFrom0to1(normalisedValue);
  }

  inline void setSync(const float newSyncModifier) noexcept
  {
    float rangeEnd = std::nextafter(100.0f, std::numeric_limits<float>::max());
    const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
    jassert(sourceRange.getRange().contains(newSyncModifier));
    const auto normalisedValue = sourceRange.convertTo0to1(newSyncModifier);
    const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
    this->syncModifier = targetRange.convertFrom0to1(normalisedValue);
  }

private:
  dmt::dsp::synth::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;

  float drive = 0.0f;
  float bias = 0.0f;
  float pwmModifier = 1.0f;
  float syncModifier = 1.0f;
  float posityCycleRatio = 0.5f;

  inline void advancePhase() noexcept
  {
    float cycleLength = sampleRate / frequency;
    float phaseDelta = twoPi / cycleLength;
    phase += phaseDelta;

    if (phase >= twoPi) {
      phase -= twoPi;
    }
  }

  inline const float getSyncedPhase(float rawPhase) const noexcept
  {
    float syncedPhase = rawPhase * syncModifier;
    while (syncedPhase >= twoPi) {
      syncedPhase -= twoPi;
    }
    return syncedPhase;
  }

  inline const float getBendedPhase(float rawPhase) const noexcept
  {
    auto bendedPhase = rawPhase;

    float positiveCycleSize = posityCycleRatio * twoPi;
    float negativeCycleRatio = 1.0f - posityCycleRatio;
    float negativeCycleSize = negativeCycleRatio * twoPi;

    if (rawPhase <= positiveCycleSize) {
      bendedPhase /= (posityCycleRatio * 2.0f);
    }

    if (rawPhase > positiveCycleSize) {
      bendedPhase = (rawPhase - positiveCycleSize) / negativeCycleSize;
      bendedPhase = bendedPhase * pi + pi;
    }
    return bendedPhase;
  }

  inline const void distortSample(float& sample) const noexcept
  {
    float magicNumber = 0.7615941559558f;
    if (drive >= 1.0f) {
      sample = Math::tanh(drive * sample);
    } else {
      float invertedDrive = 1.0f - drive;
      float wetSample = drive * Math::tanh(sample);
      float drySample = invertedDrive * sample * magicNumber;
      sample = wetSample + drySample;
    }

    sample = sample + bias;
  }
};
} // namespace synth
} // namespace dsp
} // namespace dmt
