/*
  ==============================================================================

    AdhEnvelope.h
    Created: 21 Feb 2023 12:11:41am
    Author:  Lunix

  ==============================================================================
*/

#pragma once
#include <cmath>
using namespace std;

namespace dmt {
class AhdEnvelope
{
public:
  struct Parameters
  {
    float attack = 2.5f;
    float hold = 2.5f;
    float decay = 2.5f;
    int attackScew = 0.0f;
    int decayScew = 0.0f;
  };

  enum class State
  {
    Attack,
    Hold,
    Decay,
    Idle
  };

  AhdEnvelope() {}

  void setParameters(Parameters params) { this->params = params; }
  void setSampleRate(int sampleRate) { this->sampleRate = sampleRate; }
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
    cout << "#" << sampleIndex << std::endl;
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
        cout << "Attack" << std::endl;
        return value;
      }
      case State::Hold: {
        cout << "Hold" << std::endl;
        return 1.0f;
      }
      case State::Decay: {
        cout << "Decay" << std::endl;
        float decayStart = getDecayStart();
        float normalizedPosition = (sampleIndex - decayStart) / sampleRate;
        float scew = getScew(State::Decay);
        float value = 1.0f - std::pow(normalizedPosition / params.decay, scew);
        return value;
      }
      default: {
        cout << "Default" << std::endl;
        return 0.0f;
      }
    }
  }
  float getScew(State state)
  {
    switch (state) {
      case State::Attack: {
        return getScewAsExponent(params.attackScew);
      }
      case State::Decay: {
        return getScewAsExponent(params.decayScew);
      }
      default: {
        return 1.0f;
      }
    }
  }
  float getScewAsExponent(int rawScew)
  {
    if (rawScew >= 0) {
      return 1.0f + (rawScew * 0.09);
    }
    if (rawScew < 0) {
      return 1.0f + (rawScew * 0.009);
    }
  }
  int getHoldStart() { return params.attack * sampleRate; }
  int getDecayStart() { return (params.attack + params.hold) * sampleRate; }
  int getDecayEnd()
  {
    return (params.attack + params.hold + params.decay) * sampleRate;
  }
  float sampleRate = 10.0f;
  Parameters params;
  int sampleIndex = 0;
};
}