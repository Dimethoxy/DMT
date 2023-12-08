#pragma once

#include "../dsp/synth/AnalogWaveform.h"
#include "AhdEnvelopeParameters.h"
#include "VoiceParameters.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
oscillatorParameterGroup(int index)
{
  using ParameterGroup = juce::AudioProcessorParameterGroup;

  juce::String uid = juce::String("osc" + juce::String(index));

  return juce::AudioProcessorParameterGroup(
    uid,          // group ID
    "Oscillator", // group name
    "|",          // separator
    std::make_unique<ParameterGroup>(voiceParameterGroup()),
    std::make_unique<ParameterGroup>(envelopeParameterGroup(uid, "Gain")),
    std::make_unique<ParameterGroup>(envelopeParameterGroup(uid, "Pitch")));
}
} // namespace model
} // namespace dmt