#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
waveformDistortionParameterGroup()
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  return juce::AudioProcessorParameterGroup(
    "waveformDistortion",                     // group ID
    "Waveform Distortion",                    // group name
    "|",                                      // separator
    std::make_unique<ParameterChoice>("type", // parameter ID
                                      "Type", // parameter name
                                      juce::StringArray{ "Hardclip",
                                                         "Softclip",
                                                         "Saturation",
                                                         "Atan",
                                                         "Sine",
                                                         "Cosine" }, // choices
                                      2), // defaultIndex
    std::make_unique<ParameterFloat>("preGain",
                                     "PreGain",
                                     NormalisableRange(-96.f, // rangeStart
                                                       .0f,   // rangeEnd
                                                       .1f,   // intervalValue
                                                       1.f),  // skewFactor
                                     .0f),                    // defaultValue
    std::make_unique<ParameterFloat>("drive",
                                     "Drive",
                                     NormalisableRange(.0f,  // rangeStart
                                                       10.f, // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     4.f),                   // defaultValue
    std::make_unique<ParameterFloat>("symmetry",
                                     "Symmetry",
                                     NormalisableRange(-1.f, // rangeStart
                                                       1.f,  // rangeEnd
                                                       .01f, // intervalValue
                                                       1.f), // skewFactor
                                     .0f),                   // defaultValue
    std::make_unique<ParameterFloat>("crush",
                                     "Crush",
                                     NormalisableRange(16.0f, // rangeStart
                                                       1.f,   // rangeEnd
                                                       .01f,  // intervalValue
                                                       1.f),  // skewFactor
                                     16.f)                    // defaultValue
  );
}
} // namespace model
} // namespace dmt