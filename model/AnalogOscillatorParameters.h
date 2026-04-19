#pragma once

#include "../dsp/synth/AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
analogOscillatorParameterGroup(juce::String parentUid)
{
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  using ParameterGroup = juce::AudioProcessorParameterGroup;
  using String = juce::String;

  using AnalogWaveform = dmt::dsp::synth::AnalogWaveform;

  String uid = parentUid + "AnalogOscillator";

  return juce::AudioProcessorParameterGroup(
    uid,                                            // group ID
    "Analog Waveform",                              // group name
    "|",                                            // separator
    std::make_unique<ParameterChoice>(uid + "Type", // parameter ID
                                      "Type",       // parameter name
                                      AnalogWaveform::waveformNames, // choices
                                      0), // defaultValue
    std::make_unique<ParameterFloat>(uid + "Drive",
                                     "Drive",
                                     NormalisableRange(0.f,  // rangeStart
                                                       10.f, // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     1.0f),                  // defaultValue
    std::make_unique<ParameterFloat>(uid + "Bias",
                                     "Bias",
                                     NormalisableRange(-1.f, // rangeStart
                                                       1.f,  // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     0.0f)                   // defaultValue
  );
}
} // namespace model
} // namespace dmt
