#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
globalParameterGroup()
{
  using ParameterBool = juce::AudioParameterBool;
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = "Global";

  return juce::AudioProcessorParameterGroup(
    uid,                                                       // group ID
    "Global",                                                  // group name
    "|",                                                       // separator
    std::make_unique<ParameterFloat>(uid + "Gain",             // parameter ID
                                     "Gain",                   // parameter name
                                     NormalisableRange(-100.f, // rangeStart
                                                       100.f,  // rangeEnd
                                                       0.01f,  // intervalValue
                                                       1.f),   // skewFactor
                                     0),
    std::make_unique<ParameterBool>(uid + "Bypass", // parameter ID
                                    "Bypass",       // parameter name
                                    false)          // defaultValue
  );
}
} // namespace model
} // namespace dmt