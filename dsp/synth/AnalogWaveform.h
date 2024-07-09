//==============================================================================

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace dsp {
namespace synth {
//==============================================================================
struct AnalogWaveform
{
  const float twoPi = juce::MathConstants<float>::twoPi;
  const float pi = juce::MathConstants<float>::pi;
  //============================================================================
  enum class Type
  {
    Sine,
    Saw,
    Triangle,
    Square
  };
  static const inline juce::StringArray waveformNames = { "Sine",
                                                          "Saw",
                                                          "Triangle",
                                                          "Square" };
  //============================================================================
  Type type = Type::Sine;
  //============================================================================
  inline float triangle(float x) const noexcept
  {
    while (x > twoPi)
      x -= twoPi;
    float result = 2.0f * (x / twoPi - 0.5f);
    if (result > 0.5f)
      result = 1.0f - result;
    if (result < -0.5f)
      result = -1.0f - result;
    return 2 * result;
  }
  inline float saw(float x) const noexcept
  {
    while (x > twoPi)
      x -= twoPi;
    float result = 2.0f * (x / twoPi - 0.5f);
    return result;
  }
  inline float sine(float x) const noexcept
  {
    float result = std::sin(x);
    return result;
  }
  inline float square(float x) const noexcept
  {
    float result = (sine(x) > 0.0f) ? 1.0f : -1.0f;
    return result;
  }
  //============================================================================
  inline float getSample(float x) const noexcept
  {
    switch (type) {
      case Type::Sine:
        return sine(x);
      case Type::Saw:
        return saw(x);
      case Type::Triangle:
        return triangle(x);
      case Type::Square:
        return square(x);
      default:
        // impossible to reach this point, exit with assertion
        jassert(false);
        return 0.0f;
    }
  }
  //============================================================================
};
} // namespace synth
} // namespace dsp
} // namespace dmt
//==============================================================================
