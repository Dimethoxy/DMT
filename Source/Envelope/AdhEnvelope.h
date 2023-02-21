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
    float attack = 0.015f;
    float hold = 0.1f;
    float decay = 1.0f;
    // Negative scew = curve pulled up
    int attackScew = 0;
    int decayScew = 10;
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
        float decayStart = getDecayStart();
        float decayDuration = getDecayEnd() - decayStart;
        float normalizedPosition = (sampleIndex - decayStart) / decayDuration;
        float scew = getScew(State::Decay);
        float value = 1.0f - std::pow(normalizedPosition, scew);
        return value;
        /*
    float decayStart = getDecayStart();
    float normalizedPosition = (sampleIndex - decayStart) / sampleRate;
    float scew = getScew(State::Decay);
    float value = 1.0f - std::pow(normalizedPosition / params.decay, scew);
    return value;*/
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
        return getScewAsExponent(params.attackScew);
      }
      case State::Decay: {
        return getScewAsExponent(-params.decayScew);
      }
      default: {
        return 1.0f;
      }
    }
  }
  float getScewAsExponent(int rawScew)
  {
    float exponentRange =
      3.0f; // Set the range of exponents to be between -3 and 3
    float exponent = (rawScew / 100.0f) *
                     exponentRange; // Map the skew value to the exponent range
    return std::pow(10.0f, exponent); // Convert the exponent to a suitable
                                      // value for an exponential function
  }
  int getHoldStart() { return params.attack * sampleRate; }
  int getDecayStart() { return (params.attack + params.hold) * sampleRate + 1; }
  int getDecayEnd()
  {
    return (params.attack + params.hold + params.decay) * sampleRate;
  }
  float sampleRate = 60.0f;
  Parameters params;
  int sampleIndex = 0;
};

}