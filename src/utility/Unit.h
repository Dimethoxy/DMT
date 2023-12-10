
#pragma once

#include "../dsp/effects/Distortion.h"
#include <JuceHeader.h>

namespace dmt {
struct InfoUnit
{
  enum class Type
  {
    Gain,
    Threshold,
    Cutoff,
    LfoSpeed,
    DistortionType,
    Symmetry,
    Girth,
    Drive,
    Milliseconds,
    Skew,
    Percent,
    Octave,
    Semitone,
    Fine,
    Density,
    Distribution,
    Phase,
    Detune,
    Seed,
    ModDepth,
  };

  static inline juce::String getString(dmt::InfoUnit::Type type, float value)
  {
    switch (type) {
      case dmt::InfoUnit::Type::Gain:
        return { juce::String(round(value * 10.0f) / 10.0f) + " dB" };
        break;
      case dmt::InfoUnit::Type::Threshold:
        return { juce::String(round(value * 10.0f) / 10.0f) + " dB" };
        break;
      case dmt::InfoUnit::Type::Cutoff:
        return { juce::String(round(value * 10.0f) / 10.0f) + " Hz" };
        break;
      case dmt::InfoUnit::Type::LfoSpeed:
        return { juce::String(round(value * 10.0f) / 10.0f) + " %" };
        break;
      case dmt::InfoUnit::Type::DistortionType:
        return { Distortion::getString(
          static_cast<Distortion::Type>((int)value)) };
        break;
      case dmt::InfoUnit::Type::Symmetry:
        return { juce::String(round(value * 100.0f)) + " %" };
        break;
      case dmt::InfoUnit::Type::Girth:
        if (value == 0) {
          return juce::String("Off");
        } else if (value > 0) {
          juce::String string;
          string += "Stereo ";
          string += juce::String(round(100.0f * value));
          string += "%";
          return string;
        } else {
          juce::String string;
          string += "Mono ";
          string += juce::String(round(100.0f * std::abs(value)));
          string += "%";
          return string;
        }
        break;
      case dmt::InfoUnit::Type::Drive:
        return { juce::String(round(value * 10.0f) / 10.0f) + "x" };
        break;
      case dmt::InfoUnit::Type::Milliseconds:
        return { juce::String(round(1000 * value)) + " ms" };
        break;
      case dmt::InfoUnit::Type::Skew:
        return { juce::String(round(value / 32.f * 100.f)) + "%" };
        break;
      case dmt::InfoUnit::Type::Percent:
        return { juce::String(round(value)) + " %" };
        break;
      case dmt::InfoUnit::Type::Octave:
        return { juce::String(round(value)) + " oct" };
        break;
      case dmt::InfoUnit::Type::Semitone:
        return { juce::String(round(value)) + " st" };
        break;
      case dmt::InfoUnit::Type::Fine:
        return { juce::String(round(value)) + " cent" };
        break;
      case dmt::InfoUnit::Type::Density:
        return { juce::String(value) + " Voices" };
        break;
      case dmt::InfoUnit::Type::Distribution:
        return { Distortion::getString(
          static_cast<Distortion::Type>((int)value)) };
        break;
      case dmt::InfoUnit::Type::Phase:
        return { juce::String(round(value * 10.0f) / 10.0f) + "°" };
        break;
      case dmt::InfoUnit::Type::Detune:
        return { juce::String(round(value * 10.0f) / 10.0f) + " cent" };
        break;
      case dmt::InfoUnit::Type::Seed:
        return { Distortion::getString(
          static_cast<Distortion::Type>((int)value)) };
        break;
      case dmt::InfoUnit::Type::ModDepth:
        return { juce::String(round(value * 2e4f)) + " Hz" };
        break;
      default:
        jassert(false);
        return { juce::String("ERROR") };
        break;
    }
  }
};
} // namespace dmt
