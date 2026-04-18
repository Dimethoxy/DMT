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
 * High-performance digital oscillator for real-time audio synthesis.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include "dsp/synth/DigitalWaveform.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace dsp {
namespace synth {
//==============================================================================

/**
 * @class DigitalOscillator
 * @brief High-performance digital oscillator for real-time audio synthesis.
 *
 * This class is designed for maximum real-time performance, using aggressive
 * optimizations such as constexpr, inline, noexcept, and forceinline. It
 * generates digital waveforms with various modulation capabilities.
 */
class alignas(64) DigitalOscillator
{
  using Math = juce::dsp::FastMathApproximations;
  using String = juce::String;
  using DigitalWaveform = dmt::dsp::synth::DigitalWaveform;

  static constexpr float twoPi = juce::MathConstants<float>::twoPi;
  static constexpr float pi = juce::MathConstants<float>::pi;

public:
  struct Parameters
  {
  public:
    DigitalWaveform::Type type = DigitalWaveform::Type::Sine;
    float bias = 0.0f;
    float drive = 0.0f;

  private:
    float sync = 0.0f;
    float bend = 0.0f;
    float pwm = 4.0f;

  public:
    //==============================================================================
    inline float getBend() const noexcept { return bend; }
    inline void setBend(const float _newBend) noexcept
    {
      TRACER("DigitalOscillator::setBend");
      float rangeEnd =
        std::nextafter(100.0f, std::numeric_limits<float>::max());
      const juce::NormalisableRange<float> sourceRange(-100.0f, rangeEnd);
      jassert(sourceRange.getRange().contains(_newBend));
      const auto normalisedValue = sourceRange.convertTo0to1(_newBend);
      const juce::NormalisableRange<float> targetRange(0.1f, 0.9f);
      bend = targetRange.convertFrom0to1(normalisedValue);
    }

    //==============================================================================
    inline float getPwm() const noexcept { return pwm; }
    inline void setPwm(const float _newPwm) noexcept
    {
      TRACER("DigitalOscillator::setPwm");
      float rangeEnd =
        std::nextafter(100.0f, std::numeric_limits<float>::max());
      const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
      jassert(sourceRange.getRange().contains(_newPwm));
      const auto normalisedValue = sourceRange.convertTo0to1(_newPwm);
      const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
      pwm = targetRange.convertFrom0to1(normalisedValue);
    }

    //==============================================================================
    inline float getSync() const noexcept { return sync; }
    inline void setSync(const float _newSync) noexcept
    {
      TRACER("DigitalOscillator::setSync");
      float rangeEnd =
        std::nextafter(100.0f, std::numeric_limits<float>::max());
      const juce::NormalisableRange<float> sourceRange(0.0f, rangeEnd);
      jassert(sourceRange.getRange().contains(_newSync));
      const auto normalisedValue = sourceRange.convertTo0to1(_newSync);
      const juce::NormalisableRange<float> targetRange(1.0f, 5.0f);
      sync = targetRange.convertFrom0to1(normalisedValue);
    }
  };

  //==============================================================================
  // Oscillator
public:
  inline void setParameters(const juce::AudioProcessorValueTreeState& apvts,
                            String prefix)
  {
    // Extract raw parameter values from the AudioProcessorValueTreeState
    const auto type = static_cast<DigitalWaveform::Type>(
      apvts.getRawParameterValue(prefix + "Type")->load());
    const auto bend = apvts.getRawParameterValue(prefix + "Bend")->load();
    const auto pwm = apvts.getRawParameterValue(prefix + "Pwm")->load();
    const auto sync = apvts.getRawParameterValue(prefix + "Sync")->load();
    const auto drive = apvts.getRawParameterValue(prefix + "Drive")->load();

    // These don't need mapping so we can set them directly
    params.type = type;
    params.drive = drive;

    // These need mapping so we use setters to do that
    params.setBend(bend);
    params.setPwm(pwm);
    params.setSync(sync);
  }

  //==============================================================================
  /**
   * @brief Sets the sample rate for the oscillator.
   * @param _newSampleRate The new sample rate in Hz.
   */
  inline void setSampleRate(const float _newSampleRate) noexcept
  {
    TRACER("DigitalOscillator::setSampleRate");
    sampleRate = _newSampleRate;
  }

  //==============================================================================
  /**
   * @brief Generates the next sample of the waveform.
   * @return The next sample value.
   */
  [[nodiscard]] forcedinline float getNextSample() noexcept
  {
    TRACER("DigitalOscillator::getNextSample");
    if (sampleRate <= 0.0f)
      return 0.0f;

    advancePhase();
    auto pwm = params.getPwm();
    auto syncedPhase = getSyncedPhase(phase * pwm);
    auto bendedPhase = getBendedPhase(syncedPhase);

    if (phase >= twoPi / pwm)
      return 0.0f;

    float sample = waveform.getSample(bendedPhase);
    distortSample(sample);
    return std::clamp(sample, -1.0f, +1.0f);
  }

  //==============================================================================
  /**
   * @brief Sets the frequency of the oscillator.
   * @param _newFrequency The new frequency in Hz.
   */
  inline void setFrequency(const float _newFrequency) noexcept
  {
    TRACER("DigitalOscillator::setFrequency");
    frequency = _newFrequency;
  }

  //==============================================================================
  /**
   * @brief Advances the phase of the oscillator.
   */
  forcedinline void advancePhase() noexcept
  {
    TRACER("DigitalOscillator::advancePhase");
    float cycleLength = sampleRate / frequency;
    float phaseDelta = twoPi / cycleLength;
    phase += phaseDelta;

    if (phase >= twoPi) {
      phase -= twoPi;
    }
  }

  //==============================================================================
  /**
   * @brief Computes the synced phase based on the raw phase and sync modifier.
   * @param _rawPhase The raw phase value.
   * @return The synced phase value.
   */
  forcedinline float getSyncedPhase(float _rawPhase) const noexcept
  {
    TRACER("DigitalOscillator::getSyncedPhase");
    float syncedPhase = _rawPhase * params.getSync();
    while (syncedPhase >= twoPi) {
      syncedPhase -= twoPi;
    }
    return syncedPhase;
  }

  //==============================================================================
  /**
   * @brief Computes the bended phase based on the raw phase and bend modifier.
   * @param _rawPhase The raw phase value.
   * @return The bended phase value.
   */
  forcedinline float getBendedPhase(float _rawPhase) const noexcept
  {
    TRACER("DigitalOscillator::getBendedPhase");
    auto bendedPhase = _rawPhase;

    const float bend = params.getBend();
    float positiveCycleSize = bend * twoPi;
    float negativeCycleRatio = 1.0f - bend;
    float negativeCycleSize = negativeCycleRatio * twoPi;

    if (_rawPhase <= positiveCycleSize) {
      bendedPhase /= (bend * 2.0f);
    }

    if (_rawPhase > positiveCycleSize) {
      bendedPhase = (_rawPhase - positiveCycleSize) / negativeCycleSize;
      bendedPhase = bendedPhase * pi + pi;
    }
    return bendedPhase;
  }

  //==============================================================================
  /**
   * @brief Applies distortion to the sample based on the drive and bias
   * settings.
   * @param _sample The sample value to be distorted.
   */
  forcedinline void distortSample(float& _sample) const noexcept
  {
    TRACER("DigitalOscillator::distortSample");
    constexpr float magicNumber = 0.7615941559558f;
    if (params.drive >= 1.0f) {
      _sample = Math::tanh(params.drive * _sample);
    } else {
      float invertedDrive = 1.0f - params.drive;
      float wetSample = params.drive * Math::tanh(_sample);
      float drySample = invertedDrive * _sample * magicNumber;
      _sample = wetSample + drySample;
    }

    _sample = _sample + params.bias;
  }

protected:
  //==============================================================================

private:
  DigitalWaveform waveform;
  Parameters params;
  float frequency = 50.0f;
  float sampleRate = -1.0f;
  float phase = 0.0f;
  //==============================================================================
} // namespace synth
} // namespace dsp
} // namespace dmt
