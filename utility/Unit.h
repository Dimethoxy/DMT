
#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace utility {
struct Unit
{
  enum class Type
  {
    Degree,
    Percent,
    Seed,
    Gain,
    Pan,
    Drive,
    DistortionType,
    Cents,
    Octave,
    Semitone,
    Frequency,
    Symmetry,
    Bitdepth,
    VoiceDensity,
    VoiceDistribution,
    EnvelopeSkew,
    Milliseconds,
    OscilloscopeZoom,
    OscilloscopeThickness,
    OscilloscopeHeight,
  };

  static inline juce::String getString(Type type, float value)
  {
    switch (type) {
      case Type::OscilloscopeZoom:
        return { juce::String((int)value) + juce::String("%") };
        break;
      case Type::OscilloscopeThickness:
        return { juce::String((int)value) + juce::String("px") };
        break;
      case Type::OscilloscopeHeight:
        return { juce::String((int)value) + juce::String("dB") };
        break;
      default:
        return { juce::String("ERROR") };
        break;
    }
  }
};
} // namespace utility
} // namespace dmt