#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace model {
//==============================================================================
static inline juce::AudioProcessorParameterGroup
oscilloscopeParameterGroup(juce::String parentUid)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = parentUid + "Disflux";

  return juce::AudioProcessorParameterGroup(
    uid,       // group ID
    "Disflux", // group name
    "|",       // separator
    std::make_unique<ParameterInt>(uid + "Amount",
                                   "Amount",
                                   1,  // rangeStart
                                   10, // rangeEnd
                                   1), // defaultValue
    std::make_unique<ParameterFloat>(uid + "Frequency",
                                     "Frequency",
                                     NormalisableRange(20.0f,    // rangeStart
                                                       20000.0f, // rangeEnd
                                                       1.0f,  // intervalValue
                                                       1.0f), // skewFactor
                                     1000.0f),                // defaultValue
    std::make_unique<ParameterFloat>(uid + "Pinch",
                                     "Pinch",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.0f)                    // defaultValue
  );
}
} // namespace model
} // namespace dmt