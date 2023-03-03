/*
  ==============================================================================

    AnalogOscillator.h
    Created: 2 Mar 2023 8:38:20pm
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

const float twoPi = juce::MathConstants<float>::twoPi;
const float pi = juce::MathConstants<float>::pi;

namespace dmt {
struct AnalogWaveform
{
  enum class Type
  {
    Sine,
    Saw,
    Triangle
  };
  Type type = Type::Triangle;
  inline float triangle(float x) const noexcept
  {
    float result = 2.0f * (x / twoPi - 0.5f);
    if (result > 0.5f)
      result = 1.0f - result;
    if (result < -0.5f)
      result = -1.0f - result;
    return result;
  }
  inline float saw(float x) const noexcept
  {
    float result = 2.0f * (x / twoPi - 0.5f);
    return result;
  }
  inline float sine(float x) const noexcept
  {
    float result = juce::dsp::FastMathApproximations::sin(x);
    return result;
  }
  inline float getSample(float x) const noexcept
  {
    switch (type) {
      case Type::Sine:
        return sine(x);
      case Type::Saw:
        return saw(x);
      case Type::Triangle:
        return triangle(x);
    }
  }
};
class AnalogOscillator
{
public:
  void setSampleRate(float sampleRate) noexcept
  {
    this->sampleRate = sampleRate;
  }

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

    return waveform.getSample(phase);
  }
  void setFrequency(float frequency) noexcept { this->frequency = frequency; }
  void resetPhase() { this->phase = 0.0f; }

private:
  dmt::AnalogWaveform waveform;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
};
}