#pragma once

//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace model {

//==============================================================================

static inline juce::AudioProcessorParameterGroup
hereticParameterGroup(juce::String parentUid)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  // using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = parentUid + "Heretic";

  return juce::AudioProcessorParameterGroup(
    uid,       // group ID
    "Heretic", // group name
    "|",       // separator
    std::make_unique<ParameterFloat>(uid + "Drive",
                                     "Drive",
                                     NormalisableRange(1.0f,   // rangeStart
                                                       256.0f, // rangeEnd
                                                       1.0f,   // intervalValue
                                                       0.5f),  // skewFactor
                                     3.0f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Range",
                                     "Range",
                                     NormalisableRange(0.0f,   // rangeStart
                                                       120.0f, // rangeEnd
                                                       0.1f,   // intervalValue
                                                       1.0f),  // skewFactor
                                     200.0f),                  // defaultValue
    std::make_unique<ParameterFloat>(uid + "Tone",
                                     "Tone",
                                     NormalisableRange(20.0f,    // rangeStart
                                                       20000.0f, // rangeEnd
                                                       1.0f,   // intervalValue
                                                       0.25f), // skewFactor
                                     50.0f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Feedback",
                                     "Feedback",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.2f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Mix",
                                     "Mix",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.5f)                    // defaultValue
  );
}
} // namespace model
} // namespace dmt