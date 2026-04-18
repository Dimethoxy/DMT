#pragma once

#include "../dsp/synth/DigitalOscillator.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
digitalOscillatorParameterGroup(juce::String parentUid)
{
  // using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  using ParameterGroup = juce::AudioProcessorParameterGroup;
  using String = juce::String;

  using DigitalWaveform = dmt::dsp::synth::DigitalWaveform;

  String uid = parentUid + "DigitalOscillator";

  return juce::AudioProcessorParameterGroup(
    uid,                                            // group ID
    "Waveform",                                     // group name
    "|",                                            // separator
    std::make_unique<ParameterChoice>(uid + "Type", // parameter ID
                                      "Type",       // parameter name
                                      DigitalWaveform::waveformNames, // choices
                                      2), // defaultValue
    std::make_unique<ParameterFloat>(uid + "Bend",
                                     "Bend",
                                     NormalisableRange(-100.f, // rangeStart
                                                       100.f,  // rangeEnd
                                                       .01f,   // intervalValue
                                                       1.f),   // skewFactor
                                     .0f),                     // defaultValue
    std::make_unique<ParameterFloat>(uid + "Pwm",
                                     "Pwm",
                                     NormalisableRange(.0f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .01f,  // intervalValue
                                                       1.f),  // skewFactor
                                     4.f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Sync",
                                     "Sync",
                                     NormalisableRange(0.f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .01f,  // intervalValue
                                                       1.f),  // skewFactor
                                     .0f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Drive",
                                     "Drive",
                                     NormalisableRange(0.f,  // rangeStart
                                                       10.f, // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     2.0f)                   // defaultValue
  );
}
} // namespace model
} // namespace dmt