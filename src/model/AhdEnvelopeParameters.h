#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
envelopeParameterGroup(juce::String parentUid, juce::String suffix)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = parentUid + suffix + "Env";

  return juce::AudioProcessorParameterGroup(
    uid,                                                       // group ID
    suffix + "Envelope",                                       // group name
    "|",                                                       // separator
    std::make_unique<ParameterFloat>(uid + "Attack",           // parameter ID
                                     "Attack",                 // parameter name
                                     NormalisableRange(0.0f,   // rangeStart
                                                       1.0f,   // rangeEnd
                                                       0.001f, // intervalValue
                                                       0.5f),  // skewFactor
                                     0.0f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Hold",             // parameter ID
                                     "Hold",                   // parameter name
                                     NormalisableRange(0.0f,   // rangeStart
                                                       1.0f,   // rangeEnd
                                                       0.001f, // intervalValue
                                                       0.5f),  // skewFactor
                                     0.08f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Decay",            // parameter ID
                                     "Decay",                  // parameter name
                                     NormalisableRange(0.0f,   // rangeStart
                                                       3.0f,   // rangeEnd
                                                       0.001f, // intervalValue
                                                       0.5f),  // skewFactor
                                     0.5f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Skew",             // parameter ID
                                     "Skew",                   // parameter name
                                     NormalisableRange(0.0f,   // rangeStart
                                                       32.0f,  // rangeEnd
                                                       0.1f,   // intervalValue
                                                       1.0f),  // skewFactor
                                     32.0f));                  // defaultValue
}
} // namespace model
} // namespace dmt