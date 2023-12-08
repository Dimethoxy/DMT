#pragma once

#include "../dsp/synth/AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
oscillatorParameterGroup()
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  return juce::AudioProcessorParameterGroup(
    "voiceParameters",  // group ID
    "Voice Parameters", // group name
    "|",                // separator
    std::make_unique<ParameterChoice>(
      "waveformType",                                 // parameter ID
      "Waveform",                                     // parameter name
      dmt::dsp::synth::AnalogWaveform::waveformNames, // choices
      2),                                             // defaultIndex
    std::make_unique<ParameterFloat>("oscGain",
                                     "Osc Gain",
                                     NormalisableRange(-96.f, // rangeStart
                                                       .0f,   // rangeEnd
                                                       .1f,   // intervalValue
                                                       1.f),  // skewFactor
                                     .0f),                    // defaultValue
    std::make_unique<ParameterFloat>("oscDrive",
                                     "Osc Drive",
                                     NormalisableRange(.0f,  // rangeStart
                                                       10.f, // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     4.f),                   // defaultValue
    std::make_unique<ParameterFloat>("oscBias",
                                     "Osc Bias",
                                     NormalisableRange(-1.f, // rangeStart
                                                       1.f,  // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     .0f),                   // defaultValue
    std::make_unique<ParameterFloat>("oscBend",
                                     "Osc Bend",
                                     NormalisableRange(-100.f, // rangeStart
                                                       100.f,  // rangeEnd
                                                       .1f,    // intervalValue
                                                       1.f),   // skewFactor
                                     .0f),                     // defaultValue
    std::make_unique<ParameterFloat>("oscPwm",
                                     "Osc PWM",
                                     NormalisableRange(.0f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .01f,  // intervalValue
                                                       1.f),  // skewFactor
                                     .0f),                    // defaultValue
    std::make_unique<ParameterFloat>("oscSync",
                                     "Osc Sync",
                                     NormalisableRange(0.f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .01f,  // intervalValue
                                                       1.f),  // skewFactor
                                     .0f)                     // defaultValue
  );
}
} // namespace model
} // namespace dmt