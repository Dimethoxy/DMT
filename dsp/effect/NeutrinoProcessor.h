//==============================================================================
/* ██████╗ ██╗███╗   ███╗███████╗████████╗██╗  ██╗ ██████╗ ██╗  ██╗██╗   ██╗
 * ██╔══██╗██║████╗ ████║██╔════╝╚══██╔══╝██║  ██║██╔═══██╗╚██╗██╔╝╚██╗ ██╔╝
 * ██║  ██║██║██╔████╔██║█████╗     ██║   ███████║██║   ██║ ╚███╔╝  ╚████╔╝
 * ██║  ██║██║██║╚██╔╝██║██╔══╝     ██║   ██╔══██║██║   ██║ ██╔██╗   ╚██╔╝
 * ██████╔╝██║██║ ╚═╝ ██║███████╗   ██║   ██║  ██║╚██████╔╝██╔╝ ██╗   ██║
 * ╚═════╝ ╚═╝╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * Part of the Dimethoxy Library, primarily intended for Dimethoxy plugins.
 * External use is permitted but not recommended.
 * No support or compatibility guarantees are provided.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * Neutrino Processor class for processing audio buffers to generate a kick drum
 * sound.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>
#include <utility/Settings.h>

namespace dmt {
namespace dsp {
namespace effect {

//==============================================================================

/**
 * @brief Neutrino Processor
 *
 * This class processes audio buffers to generate a kick drum sound.
 */
class alignas(64) NeutrinoProcessor
{
  constexpr static float MIN_FREQUENCY = 20.0f;
  constexpr static float MAX_FREQUENCY = 20000.0f;

  using AudioBuffer = juce::AudioBuffer<float>;

public:
  NeutrinoProcessor(juce::AudioProcessorValueTreeState& _apvts)
    : apvts(_apvts)
  {
  }

  //==============================================================================
  /**
   * @brief Prepares the processor with the given sample rate.
   *
   * @param _newSampleRate The sample rate.
   */
  inline void prepare(const double _newSampleRate) noexcept
  {
    sampleRate = static_cast<float>(_newSampleRate);
  }

  inline void processBlock(AudioBuffer& _buffer) noexcept
  {
    if (sampleRate <= 0.0f) {
      return;
    }

    // Load parameters
    // exampleParameter = apvts.getRawParameterValue("example")->load();
  }

private:
  //==============================================================================
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;

  float exampleParameter = 0.0f;

  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedExample;
};

//==============================================================================
} // namespace effect
} // namespace dsp
} // namespace dmt