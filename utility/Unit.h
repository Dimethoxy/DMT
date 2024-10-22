
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
    DisfluxAmount,
    DisfluxSpread,
    DisfluxFrequency,
    DisfluxPinch,
    DisfluxMix
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
      case Type::DisfluxAmount:
        return { juce::String((int)value) + juce::String("x") };
        break;
      case Type::DisfluxSpread:
        return { juce::String((int)value) + juce::String("Hz") };
        break;
      case Type::DisfluxFrequency:
        return { juce::String((int)value) + juce::String("Hz") };
        break;
      case Type::DisfluxPinch:
        return { juce::String((int)(value * 100)) + juce::String("%") };
        break;
      case Type::DisfluxMix:
        return { juce::String((int)(value * 100)) + juce::String("%") };
        break;
      default:
        return { juce::String("ERROR") };
        break;
    }
  }
};
} // namespace utility
} // namespace dmt