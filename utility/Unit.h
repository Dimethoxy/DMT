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

    // Disflux
    DisfluxAmount,
    DisfluxSpread,
    DisfluxFrequency,
    DisfluxPinch,
    DisfluxMix,

    // Heretik Main Panel
    HeretikPreGain,
    HeretikStereo,
    HeretikRange,
    HeretikDistortion,
    HeretikMix,

    // Heretik Drive Panel
    HeretikDrive,
    HeretikDriveType,
    HeretikDriveBias,

    // Heretik Feedback Panel
    HeretikFeedback,
    HeretikFeedbackFilterSlope,
    HeretikFeedbackFilterCutoff,
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
        // Map 0.5–16.0 to 0–100%
        return juce::String(static_cast<int>(
                 ((_value - 0.5f) / (16.0f - 0.5f)) * 100.0f)) +
               juce::String("%");
        break;
      case Type::DisfluxMix:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;

        // Heretik
      case Type::HeretikPreGain:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("dB");
        break;
      case Type::HeretikStereo:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::HeretikRange:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 1000.0f))) +
               juce::String(juce::CharPointer_UTF8("\u00B5s"));
        break;
      case Type::HeretikDistortion:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::HeretikMix:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;

      case Type::HeretikDrive:
        return juce::String(_value, 2) + juce::String("x");
        break;
      case Type::HeretikDriveType:
        // Return the name od the current parameter value
        return juce::String(_value);
        break;
      case Type::HeretikDriveBias:
        return juce::String(static_cast<int>(static_cast<float>(_value))) +
               juce::String("%");
        break;
      case Type::HeretikFeedback:
        return juce::String(
                 static_cast<int>(static_cast<float>(_value * 100.0f))) +
               juce::String("%");
        break;
      case Type::HeretikFeedbackFilterSlope:
        return juce::String(
          multiFilterSlopes[static_cast<int>(static_cast<float>(_value))]);
        break;
      case Type::HeretikFeedbackFilterCutoff: {
        const int minHz = 20;
        const int maxHz = 20000;
        const float minInput = 0.0f;
        const float maxInput = 10.0f;
        const float scaledValue =
          juce::jmap<float>(_value, minInput, maxInput, minHz, maxHz);
        return juce::String(static_cast<int>(static_cast<float>(scaledValue))) +
               juce::String("Hz");
        break;
      }
      default:
        return juce::String("ERROR");
        break;
    }
  }

  //==============================================================================

  static constexpr const char* multiFilterSlopes[]{
    "LP 12dB/Oct", "LP 24dB/Oct", "LP 36dB/Oct", "LP 48dB/Oct",
    "BP 12dB/Oct", "BP 24dB/Oct", "BP 36dB/Oct", "BP 48dB/Oct",
    "HP 12dB/Oct", "HP 24dB/Oct", "HP 36dB/Oct", "HP 48dB/Oct"
  };

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Unit)
};

} // namespace utility
} // namespace dmt