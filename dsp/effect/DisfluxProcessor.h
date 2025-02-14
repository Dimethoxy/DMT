//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ██   ███████ ██    ██   ███     ████
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
 * modify this code under the terms of this license. You must adhere GPLv3
 * license for any project using this code or parts of it. Your are not allowed
 * to use this code in any closed-source project.
 *
 * Description:
 * Disflux Processor class for processing audio buffers with a series of
 * all-pass filters.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>
#include <utility/Settings.h>

//==============================================================================

namespace dmt {
namespace dsp {
namespace effect {

//==============================================================================

/**
 * @brief Disflux Processor
 *
 * This class processes audio buffers with a series of all-pass filters.
 */
class alignas(64) DisfluxProcessor
{
  constexpr static int FILTER_AMOUNT = 256;
  constexpr static float MIN_FREQUENCY = 20.0f;
  constexpr static float MAX_FREQUENCY = 20000.0f;
  using AudioBuffer = juce::AudioBuffer<float>;
  using Filter = juce::IIRFilter;
  using FilterArray = std::array<Filter, FILTER_AMOUNT>;

public:
  //==============================================================================
  /**
   * @brief Constructs a DisfluxProcessor with the given parameters.
   *
   * @param _apvts The AudioProcessorValueTreeState containing the parameters.
   */
  DisfluxProcessor(juce::AudioProcessorValueTreeState& _apvts) noexcept
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
    setCoefficients();
  }

  //==============================================================================
  /**
   * @brief Processes an audio buffer.
   *
   * @param _buffer The audio buffer.
   */
  inline void processBlock(AudioBuffer& _buffer) noexcept
  {
    if (sampleRate <= 0.0f) {
      return;
    }

    const int newAmount = apvts.getRawParameterValue("DisfluxAmount")->load();
    const int newSpread = apvts.getRawParameterValue("DisfluxSpread")->load();
    const auto newFrequency =
      apvts.getRawParameterValue("DisfluxFrequency")->load();
    const auto newPinch = apvts.getRawParameterValue("DisfluxPinch")->load();
    const auto mix = apvts.getRawParameterValue("DisfluxMix")->load();

    if (amount != newAmount || spread != newSpread ||
        !juce::approximatelyEqual(frequency, newFrequency) ||
        !juce::approximatelyEqual(pinch, newPinch)) {
      amount = newAmount;
      spread = newSpread;
      frequency = newFrequency;
      pinch = newPinch;
      setCoefficients();
    }

    for (size_t sample = 0; sample < _buffer.getNumSamples(); ++sample) {
      auto const leftDry = _buffer.getSample(0, sample);
      auto const rightDry = _buffer.getSample(1, sample);
      auto left = leftDry;
      auto right = rightDry;

      for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
        left = leftFilters[filterIndex].processSingleSampleRaw(left);
        right = rightFilters[filterIndex].processSingleSampleRaw(right);
      }

      const auto wetGain = mix;
      const auto dryGain = 1.0f - mix;
      left = (left * wetGain) + (leftDry * dryGain);
      right = (right * wetGain) + (rightDry * dryGain);
      _buffer.setSample(0, sample, left);
      _buffer.setSample(1, sample, right);
    }
  }

protected:
  //==============================================================================
  /**
   * @brief Sets the coefficients for the filters.
   */
  inline void setCoefficients() noexcept
  {
    const float spreadAmount = static_cast<float>(spread);
    const float rangeStartFrequency = juce::jlimit(
      MIN_FREQUENCY, MAX_FREQUENCY, frequency - (spreadAmount / 2.0f));
    const float rangeEndFrequency = juce::jlimit(
      MIN_FREQUENCY, MAX_FREQUENCY, frequency + (spreadAmount / 2.0f));

    const float logStartFrequency = std::log(rangeStartFrequency);
    const float logEndFrequency = std::log(rangeEndFrequency);
    const float logFrequencyDelta = logEndFrequency - logStartFrequency;

    for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
      const float logFrequencyOffsetFactor =
        (amount == 1) ? 0.5f : static_cast<float>(filterIndex) / (amount - 1);

      const float logFrequency = std::exp(
        logStartFrequency + (logFrequencyDelta * logFrequencyOffsetFactor));

      const auto coefficients = juce::IIRCoefficients::makeAllPass(
        static_cast<double>(sampleRate), logFrequency, pinch);

      leftFilters[filterIndex].setCoefficients(coefficients);
      rightFilters[filterIndex].setCoefficients(coefficients);
    }
  }

private:
  //==============================================================================
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;
  int amount = 1;
  int spread = 0;
  float frequency = 800.0f;
  float pinch = 1.0f;
  FilterArray leftFilters;
  FilterArray rightFilters;
};

//==============================================================================
} // namespace effect
} // namespace dsp
} // namespace dmt