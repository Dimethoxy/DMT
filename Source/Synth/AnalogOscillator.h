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
class AnalogOscillator
{
public:
  void setSampleRate(float sampleRate) { this->sampleRate = sampleRate; }
  float getNextSample()
  {
    if (sampleRate <= 0.0f)
      return 0.0f;

    float cycleLength = sampleRate / frequency;
    float phaseDelta = twoPi / cycleLength;
    phase += phaseDelta;

    if (phase >= twoPi) {
      phase -= twoPi;
    }

    return std::sin(phase);
  }
  void setFrequency(float frequency) { this->frequency = frequency; }
  void resetPhase() { this->phase = 0.0f; }

private:
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
};
}

// float wave(float x)
//{
//   auto signBit = std::signbit(x) ? -1 : 1;
//   auto result = x * signBit / pi - pi;
//   return std::sin(x);
// }