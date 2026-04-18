#pragma once

//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace dsp {
namespace synth {

//==============================================================================
/**
 * @brief Uses PolyBLEP to emulate analog waveforms.
 */
struct AnalogWaveform
{
  static constexpr float twoPi = juce::MathConstants<float>::twoPi;
  static constexpr float pi = juce::MathConstants<float>::pi;

  //==============================================================================
  /**
   * @brief Enumeration of waveform types.
   */
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

  //==============================================================================

  Type type = Type::Sine;
  float delta = 0.0f;
  float lastPhase = 0.0f;

  //==============================================================================
  /**
   * @brief Generate a triangle waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float triangle(float _x) const noexcept
  {
    _x = juce::jlimit(0.0f, 1.0f, _x);
    return _x < 0.5f ? 4.0f * _x - 1.0f : 3.0f - 4.0f * _x;
  }

  //==============================================================================
  /**
   * @brief Generate a saw waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float saw(float _x) const noexcept
  {
    _x = juce::jlimit(0.0f, 1.0f, _x);
    return 2.0f * _x - 1.0f;
  }

  //==============================================================================
  /**
   * @brief Generate a sine waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float sine(float _x) const noexcept
  {
    _x = juce::jlimit(0.0f, 1.0f, _x);
    return std::sin(twoPi * _x);
  }

  //==============================================================================
  /**
   * @brief Generate a square waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float square(float _x) const noexcept
  {
    _x = juce::jlimit(0.0f, 1.0f, _x);
    return _x < 0.5f ? 1.0f : -1.0f;
  }

  //==============================================================================
  /**
   * @brief Get the waveform sample based on the current type.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  [[nodiscard]] inline float getSample(float _x) noexcept
  {
    float sample = 0.0f;
    bool discontinuity = false;
    float t = 0.0f;

    // Detect phase discontinuity (wraparound)
    if (_x < lastPhase) {
      discontinuity = true;
      t = _x / delta;
    }

    switch (type) {
      case Type::Sine:
        sample = sine(_x);
        break;
      case Type::Saw:
        sample = saw(_x);
        if (discontinuity && std::abs(t) < 1.0f)
          sample += polyBlepCorrection(t);
        break;
      case Type::Triangle:
        sample = triangle(_x);
        if (discontinuity && std::abs(t) < 1.0f)
          sample += polyBlepCorrection(t);
        break;
      case Type::Square:
        sample = square(_x);
        if (discontinuity && std::abs(t) < 1.0f)
          sample += polyBlepCorrection(t);
        break;
    }

    lastPhase = _x;
    return sample;
  }

  //==============================================================================
  /**
   * @brief PolyBLEP correction polynomial.
   * @param _t Normalized time around discontinuity (-1 to 1).
   * @return Correction sample.
   */
  [[nodiscard]] inline float polyBlepCorrection(float _t) const noexcept
  {
    if (_t > 0.0f && _t < 1.0f)
      return _t * _t - _t;
    if (_t < 0.0f && _t > -1.0f)
      return _t * _t + _t;
    return 0.0f;
  }

  //==============================================================================
};

//==============================================================================

} // namespace synth
} // namespace dsp
} // namespace dmt