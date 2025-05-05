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
class alignas(64) HereticProcessor
{
  using AudioBuffer = juce::AudioBuffer<float>;
  using Filter = juce::IIRFilter;

public:
  //==============================================================================
  /**
   * @brief Constructs a DisfluxProcessor with the given parameters.
   *
   * @param _apvts The AudioProcessorValueTreeState containing the parameters.
   */
  HereticProcessor(juce::AudioProcessorValueTreeState& _apvts) noexcept
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

    const int tempDrive = apvts.getRawParameterValue("HereticDrive")->load();
    const int tempRange = apvts.getRawParameterValue("HereticRange")->load();
    const int tempTone = apvts.getRawParameterValue("HereticTone")->load();
    const int tempFeedback =
      apvts.getRawParameterValue("HereticFeedback")->load();
    const int tempMix = apvts.getRawParameterValue("HereticMix")->load();
  }

private:
  //==============================================================================
  juce::AudioProcessorValueTreeState& apvts;
  float sampleRate = -1.0f;
};

//==============================================================================
} // namespace effect
} // namespace dsp
} // namespace dmt