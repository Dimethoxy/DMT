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
  return juce::AudioProcessorParameterGroup(
    "osc" + juce::String(index), // group ID
    "Oscillator",                // group name
    "|",                         // separator
    std::make_unique<ParameterGroup>(voiceParameterGroup()),
    std::make_unique<ParameterGroup>(envelopeParameterGroup("gain")),
    std::make_unique<ParameterGroup>(envelopeParameterGroup("pitch")));
}
} // namespace model
} // namespace dmt