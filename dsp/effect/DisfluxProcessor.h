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
  constexpr static int FILTER_AMOUNT = 128;
  constexpr static float MIN_FREQUENCY = 20.0f;
  constexpr static float MAX_FREQUENCY = 20000.0f;

  // Smoothing times (seconds) for each parameter
  constexpr static float FREQUENCY_SMOOTH_TIME = 0.2f;
  constexpr static float SPREAD_SMOOTH_TIME = 0.05f;
  constexpr static float PINCH_SMOOTH_TIME = 0.2f;
  constexpr static float MIX_SMOOTH_TIME = 0.05f;

  // Smoothing interval (samples)
  constexpr static int SMOOTHING_INTERVAL = 32;

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

    smoothedFrequency.reset(sampleRate, FREQUENCY_SMOOTH_TIME);
    smoothedSpread.reset(sampleRate, SPREAD_SMOOTH_TIME);
    smoothedPinch.reset(sampleRate, PINCH_SMOOTH_TIME);
    smoothedMix.reset(sampleRate, MIX_SMOOTH_TIME);

    // Set initial values
    smoothedFrequency.setCurrentAndTargetValue(frequency);
    smoothedSpread.setCurrentAndTargetValue(static_cast<float>(spread));
    smoothedPinch.setCurrentAndTargetValue(pinch);
    smoothedMix.setCurrentAndTargetValue(1.0f);

    setCoefficients(frequency, static_cast<float>(spread), pinch);
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

    // Load parameters
    const int newAmount = apvts.getRawParameterValue("DisfluxAmount")->load();
    const int newSpread = apvts.getRawParameterValue("DisfluxSpread")->load();
    const auto newFrequency =
      apvts.getRawParameterValue("DisfluxFrequency")->load();
    const auto newPinch = apvts.getRawParameterValue("DisfluxPinch")->load();
    const auto newMix = apvts.getRawParameterValue("DisfluxMix")->load();

    // Set smoothing targets
    smoothedFrequency.setTargetValue(newFrequency);
    smoothedSpread.setTargetValue(static_cast<float>(newSpread));
    smoothedPinch.setTargetValue(newPinch);
    smoothedMix.setTargetValue(newMix);

    // Only reset filters if amount changes
    bool needUpdateCoeffs = false;

    // Check if any parameters have changed
    if (spread != newSpread) {
      spread = newSpread;
      needUpdateCoeffs = true;
    }
    if (!juce::approximatelyEqual(frequency, newFrequency)) {
      frequency = newFrequency;
      needUpdateCoeffs = true;
    }
    if (!juce::approximatelyEqual(pinch, newPinch)) {
      pinch = newPinch;
      needUpdateCoeffs = true;
    }

    // If the amount of filters has changed, reset the filters
    if (amount != newAmount) {
      amount = newAmount;
      for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
        leftFilters[filterIndex].reset();
        rightFilters[filterIndex].reset();
      }
      needUpdateCoeffs = false;
      smoothedFrequency.skip(
        static_cast<int>(sampleRate * FREQUENCY_SMOOTH_TIME));
      smoothedSpread.skip(static_cast<int>(sampleRate * SPREAD_SMOOTH_TIME));
      smoothedPinch.skip(static_cast<int>(sampleRate * PINCH_SMOOTH_TIME));
      smoothedMix.skip(static_cast<int>(sampleRate * MIX_SMOOTH_TIME));
    }

    int numSamples = _buffer.getNumSamples();
    int smoothingCountdown = smoothingIntervalCountdown;
    float currentFrequency = smoothedFrequency.getCurrentValue();
    float currentSpread = smoothedSpread.getCurrentValue();
    float currentPinch = smoothedPinch.getCurrentValue();

    for (int sample = 0; sample < numSamples; ++sample) {
      // Smoothing interval logic: update filter coefficients every
      // SMOOTHING_INTERVAL samples
      if (smoothingCountdown <= 0) {
        currentFrequency = smoothedFrequency.getCurrentValue();
        currentSpread = smoothedSpread.getCurrentValue();
        currentPinch = smoothedPinch.getCurrentValue();
        setCoefficients(currentFrequency, currentSpread, currentPinch);
        smoothingCountdown = SMOOTHING_INTERVAL;
      }

      // Advance smoothing values for each sample
      currentFrequency = smoothedFrequency.getNextValue();
      currentSpread = smoothedSpread.getNextValue();
      currentPinch = smoothedPinch.getNextValue();

      smoothingCountdown--;

      auto const leftDry = _buffer.getSample(0, sample);
      auto const rightDry = _buffer.getSample(1, sample);
      auto left = leftDry;
      auto right = rightDry;

      for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
        left = leftFilters[filterIndex].processSingleSampleRaw(left);
        right = rightFilters[filterIndex].processSingleSampleRaw(right);
      }

      const auto wetGain = smoothedMix.getNextValue();
      const auto dryGain = 1.0f - wetGain;
      left = (left * wetGain) + (leftDry * dryGain);
      right = (right * wetGain) + (rightDry * dryGain);
      _buffer.setSample(0, sample, left);
      _buffer.setSample(1, sample, right);
    }
    smoothingIntervalCountdown = smoothingCountdown;
  }

protected:
  //==============================================================================
  /**
   * @brief Sets the coefficients for the filters.
   */
  inline void setCoefficients(float freq, float sprd, float pnch) noexcept
  {
    const float spreadAmount = sprd;
    const float rangeStartFrequency =
      juce::jlimit(MIN_FREQUENCY, MAX_FREQUENCY, freq - (spreadAmount / 2.0f));
    const float rangeEndFrequency =
      juce::jlimit(MIN_FREQUENCY, MAX_FREQUENCY, freq + (spreadAmount / 2.0f));

    const float logStartFrequency = std::log(rangeStartFrequency);
    const float logEndFrequency = std::log(rangeEndFrequency);
    const float logFrequencyDelta = logEndFrequency - logStartFrequency;

    for (size_t filterIndex = 0; filterIndex < amount; ++filterIndex) {
      const float logFrequencyOffsetFactor =
        (amount == 1) ? 0.5f : static_cast<float>(filterIndex) / (amount - 1);

      const float logFrequency = std::exp(
        logStartFrequency + (logFrequencyDelta * logFrequencyOffsetFactor));

      const auto coefficients = juce::IIRCoefficients::makeAllPass(
        static_cast<double>(sampleRate), logFrequency, pnch);

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

  // Smoothing
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>
    smoothedFrequency;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative>
    smoothedSpread;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedPinch;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedMix;
  int smoothingIntervalCountdown = 0;
};

//==============================================================================
} // namespace effect
} // namespace dsp
} // namespace dmt