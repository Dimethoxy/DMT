#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
waveformDistortionParameterGroup(juce::String parentUid)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = parentUid + "WaveformDistortion";

  return juce::AudioProcessorParameterGroup(
    uid,                                            // group ID
    "Waveform Distortion",                          // group name
    "|",                                            // separator
    std::make_unique<ParameterChoice>(uid + "Type", // parameter ID
                                      "Type",       // parameter name
                                      juce::StringArray{ "Hardclip",
                                                         "Softclip",
                                                         "Saturation",
                                                         "Atan",
                                                         "Sine",
                                                         "Cosine" }, // choices
                                      2), // defaultIndex
    std::make_unique<ParameterFloat>(uid + "PreGain",
                                     "PreGain",
                                     NormalisableRange(-96.f, // rangeStart
                                                       .0f,   // rangeEnd
                                                       .1f,   // intervalValue
                                                       1.f),  // skewFactor
                                     .0f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Drive",
                                     "Drive",
                                     NormalisableRange(.0f,  // rangeStart
                                                       10.f, // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     4.f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Symmetry",
                                     "Symmetry",
                                     NormalisableRange(-1.f, // rangeStart
                                                       1.f,  // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     .0f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Crush",
                                     "Crush",
                                     NormalisableRange(0.0f, // rangeStart
                                                       15.f, // rangeEnd
                                                       .1f,  // intervalValue
                                                       1.f), // skewFactor
                                     16.f)                   // defaultValue
  );
}
} // namespace model
} // namespace dmt