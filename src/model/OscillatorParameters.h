#pragma once

#include "../dsp/synth/AnalogWaveform.h"
#include "AhdEnvelopeParameters.h"
#include "DistortionParameters.h"
#include "OscSendParameterGroup.h"
#include "VoiceParameters.h"
#include "WaveformParameters.h"
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
    std::make_unique<ParameterGroup>(waveformParameterGroup(uid)),
    std::make_unique<ParameterGroup>(voiceParameterGroup(uid)),
    std::make_unique<ParameterGroup>(distortionParameterGroup(uid)),
    std::make_unique<ParameterGroup>(envelopeParameterGroup(uid, "Gain")),
    std::make_unique<ParameterGroup>(envelopeParameterGroup(uid, "Pitch")),
    std::make_unique<ParameterGroup>(oscSendParameterGroup(uid, "A")),
    std::make_unique<ParameterGroup>(oscSendParameterGroup(uid, "B")),
    std::make_unique<ParameterGroup>(oscSendParameterGroup(uid, "C")));
}
} // namespace model
} // namespace dmt