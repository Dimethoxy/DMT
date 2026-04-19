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

  //==============================================================================
  /**
   * @brief Generate a triangle waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float triangle(float _x) const noexcept {}

  //==============================================================================
  /**
   * @brief Generate a saw waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float saw(float _x) const noexcept {}

  //==============================================================================
  /**
   * @brief Generate a sine waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float sine(float _x) const noexcept {}

  //==============================================================================
  /**
   * @brief Generate a square waveform sample.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  inline float square(float _x) const noexcept {}

  //==============================================================================
  /**
   * @brief Get the waveform sample based on the current type.
   * @param _x The phase of the waveform.
   * @return The waveform sample.
   */
  [[nodiscard]] inline float getSample(float _x, float _delta) noexcept {}

  //==============================================================================
};

//==============================================================================

} // namespace synth
} // namespace dsp
} // namespace dmt