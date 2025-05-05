//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ███████ ██    ██   ███     ████
 * ██   ██ ██ ██  ██  ██ ██         ██    ██   ██ ██    ██  ██ ██     ██
 * ██████  ██ ██      ██ ███████    ██    ██   ██  ██████  ██   ██    ██
 *
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * This file is part of the Dimethoxy Library, a collection of essential
 * classes used across various Dimethoxy projects.
 * These files are primarily designed for internal use within our repositories.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * Provides a strongly-typed enumeration for units and a utility for converting
 * unit values to formatted strings for display. Designed for real-time audio
 * parameter visualization and UI feedback.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace utility {

//==============================================================================
/**
 * @brief Utility struct for handling unit types and formatting their values.
 *
 * @details
 * This struct provides a strongly-typed enumeration for various unit types
 * relevant to audio and DSP parameters. It also offers a static utility
 * function to convert a unit value to a formatted string suitable for UI
 * display. The design ensures type safety and extensibility for future units.
 *
 * Intended for use in real-time audio applications where formatted parameter
 * values must be displayed efficiently and safely.
 */
struct alignas(8) Unit
{
  //==============================================================================
  /**
   * @brief Enumeration of supported unit types.
   *
   * @details
   * Each enumerator represents a distinct unit relevant to audio processing
   * or UI display. The order and naming are stable for serialization and
   * switch-based logic.
   */
  enum class Type : int32_t
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
    DisfluxMix,
    HereticDrive,
    HereticRange,
    HereticTone,
    HereticFeedback,
    HereticMix,
  };

  //==============================================================================
  /**
   * @brief Converts a unit value to a formatted string for display.
   *
   * @param _type The unit type to format.
   * @param _value The value to be formatted.
   * @return Formatted string representing the value with its unit.
   *
   * @details
   * This function is optimized for real-time usage and avoids heap allocations
   * where possible. Explicit casts are used for clarity and type safety.
   * Edge cases (such as unknown types) return "ERROR" for debugging.
   *
   * The formatting logic is intentionally explicit to avoid ambiguity and
   * ensure correct display in all UI contexts.
   */
  [[nodiscard]] static inline juce::String getString(Type _type,
                                                     float _value) noexcept
  {
    switch (_type) {
      case Type::OscilloscopeZoom:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("%");
        break;
      case Type::OscilloscopeThickness:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("px");
        break;
      case Type::OscilloscopeHeight:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("dB");
        break;
      case Type::DisfluxAmount:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("x");
        break;
      case Type::DisfluxSpread:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("Hz");
        break;
      case Type::DisfluxFrequency:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("Hz");
        break;
      case Type::DisfluxPinch:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::DisfluxMix:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::HereticDrive:
        return juce::String(_value, 2) + juce::String("x");
        break;
      case Type::HereticRange:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 1000.0f))) +
               juce::String("\u00B5s").substring(1, 3);
        break;
      case Type::HereticTone:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("Hz");
        break;
      case Type::HereticFeedback:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::HereticMix:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      default:
        return juce::String("ERROR");
        break;
    }
  }

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Unit)
};

} // namespace utility
} // namespace dmt