
#pragma once

#include "../Utility/Distortion.h"
#include <JuceHeader.h>

namespace dmt {
struct InfoUnit
{
  static enum class Type {
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
        } else if (value < 0) {
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
      default:
        return { juce::String("ERROR") };
        break;
    }
  }
};
}
