/*
  ==============================================================================

    AdhEnvelope.h
    Created: 21 Feb 2023 12:11:41am
    Author:  Lunix

  ==============================================================================
*/

#pragma once
#include "utility/Math.h"

namespace dmt {
namespace dsp {
namespace envelope {
class AhdEnvelope
{
public:
  struct Parameters
  {
    float attack = 0.015f;
    float hold = 0.08f;
    float decay = 0.5f;

    // Negative scew = curve pulled up
    float attackScew = 0;
    float decayScew = 10;
  };

  enum class State
  {
    Attack,
    Hold,
    Decay,
    Idle
  };

  AhdEnvelope() {}

  void setParameters(Parameters& newParams) { this->params = newParams; }
  void setSampleRate(float newSampleRate) { this->sampleRate = newSampleRate; }
  void noteOn() { sampleIndex = 0; }

  State getState()
  {
    if (sampleIndex < getHoldStart())
      return State::Attack;
    if (sampleIndex < getDecayStart())
      return State::Hold;
    if (sampleIndex < getDecayEnd())
      return State::Decay;
    else
      return State::Idle;
  }

  float getNextSample()
  {
    auto state = getState();
    float result = getValue(state);
    sampleIndex++;
    return result;
  }

private:
  float getValue(State state)
  {
    switch (state) {
      case State::Attack: {
        float normalizedPosition = sampleIndex / sampleRate;
        float scew = getScew(State::Attack);
        float value = std::pow(normalizedPosition / params.attack, scew);
        return value;
      }
      case State::Hold: {
        return 1.0f;
      }
      case State::Decay: {
        float decayStart = (float)getDecayStart();
        float normalizedPosition = (sampleIndex - decayStart) / sampleRate;
        float scew = getScew(State::Decay);
        float value = 1.0f - std::pow(normalizedPosition / params.decay, scew);
        return value;
      }
      default: {
        return 0.0f;
      }
    }
  }
  float getScew(State state)
  {
    switch (state) {
      case State::Attack: {
        return dmt::math::linearToExponent(params.attackScew);
      }
      case State::Decay: {
        return dmt::math::linearToExponent(-params.decayScew);
      }
      default: {
        return 1.0f;
      }
    }
  }
  int getHoldStart() { return (int)(params.attack * sampleRate); }
  int getDecayStart()
  {
    float rawDecayDelay = params.attack + params.hold;
    int decayDelay = (int)(rawDecayDelay * sampleRate);
    int decayStart = decayDelay + 1;
    return decayStart;
  }
  int getDecayEnd()
  {
    float rawDecayEnd = (params.attack + params.hold + params.decay);
    int decayEnd = (int)(rawDecayEnd * sampleRate);
    return decayEnd;
  }
  float sampleRate = -1.0f;
  Parameters params;
  int sampleIndex = 0;
};

} // namespace envelope
} // namespace dsp
} // namespace dmt
