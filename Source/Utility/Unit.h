
#pragma once

#include "Distortion.h"
#include <JuceHeader.h>

namespace dmt {
struct InfoUnit
{
  static inline enum class Type {
    Gain,
    Threshold,
    Cutoff,
    LfoSpeed,
    DistortionType,
    Symmetry,
    Girth,
    Drive
  };

  static inline juce::String getString(dmt::InfoUnit::Type type, float value)
  {
    switch (type) {
      case Type::Gain:
        return { juce::String(round(value * 10.0f) / 10.0f) + " dB" };
        break;
      case Type::Threshold:
        return { juce::String(round(value * 10.0f) / 10.0f) + " dB" };
        break;
      case Type::Cutoff:
        return { juce::String(round(value * 10.0f) / 10.0f) + " Hz" };
        break;
      case Type::LfoSpeed:
        return { juce::String(round(value * 10.0f) / 10.0f) + " %" };
        break;
      case Type::DistortionType:
        return { dmt::Distortion::getString(
          static_cast<dmt::Distortion::Type>((int)value)) };
        break;
      case Type::Symmetry:
        return { juce::String(round(value * 100.0f)) + " %" };
        break;
      case Type::Girth:
        if (value == 0) {
          return juce::String("Off");
        } else if (value > 0) {
          juce::String string;
          string += "Stereo ";
          string += juce::String(round(100.0f * value));
          string += "%";
          return string;
        } else if (value < 0) {
          juce::String string;
          string += "Mono ";
          string += juce::String(round(100.0f * std::abs(value)));
          string += "%";
          return string;
        }
        break;
      case Type::Drive:
        return { juce::String(round(value * 10.0f) / 10.0f) + "x" };
        break;
      default:
        return { juce::String("ERROR") };
        break;
    }
  }
};
}