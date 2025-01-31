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
 * Get the options for the properties file with predefined settings.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <algorithm>
#include <cmath>

//==============================================================================

namespace dmt {
namespace dsp {
namespace filter {

//==============================================================================

/**
 * @brief Simple low-pass filter
 *
 * This class implements a simple low-pass filter using a one-pole design.
 */
class LowPassFilter
{
public:
  /**
   * @brief Set the cutoff frequency of the filter.
   * @param _cutoffFrequency The cutoff frequency in Hz.
   * @param _sampleRate The sample rate in Hz.
   */
  inline void setCutoffFrequency(const float _cutoffFrequency,
                                 const float _sampleRate) noexcept
  {
    const float RC =
      1.0f / (2.0f * static_cast<float>(M_PI) * _cutoffFrequency);
    alpha = RC / (RC + (1.0f / _sampleRate));
  }

  /**
   * @brief Process a single sample through the filter.
   * @param _input The input sample.
   * @return The filtered sample.
   */
  [[nodiscard]] inline float processSample(const float _input) noexcept
  {
    previousOutput = alpha * _input + (1.0f - alpha) * previousOutput;
    return previousOutput;
  }

private:
  float alpha = 0.0f;
  float previousOutput = 0.0f;
};

//==============================================================================

} // namespace filter
} // namespace dsp
} // namespace dmt

//==============================================================================
