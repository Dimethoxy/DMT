#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace model {
//==============================================================================
static inline juce::AudioProcessorParameterGroup
disfluxParameterGroup(juce::String parentUid)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  // using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;

  juce::String uid = parentUid + "Disflux";

  return juce::AudioProcessorParameterGroup(
    uid,       // group ID
    "Disflux", // group name
    "|",       // separator
    std::make_unique<ParameterFloat>(uid + "Amount",
                                     "Amount",
                                     NormalisableRange(1.0f,   // rangeStart
                                                       256.0f, // rangeEnd
                                                       1.0f,   // intervalValue
                                                       0.5f),  // skewFactor
                                     3.0f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Spread",
                                     "Spread",
                                     NormalisableRange(0.0f,     // rangeStart
                                                       10000.0f, // rangeEnd
                                                       1.0f,  // intervalValue
                                                       0.5f), // skewFactor
                                     200.0f),                 // defaultValue
    std::make_unique<ParameterFloat>(uid + "Frequency",
                                     "Frequency",
                                     NormalisableRange(20.0f,    // rangeStart
                                                       20000.0f, // rangeEnd
                                                       1.0f,   // intervalValue
                                                       0.25f), // skewFactor
                                     50.0f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Pinch",
                                     "Pinch",
                                     NormalisableRange(0.5f,   // rangeStart
                                                       16.0f,  // rangeEnd
                                                       0.001f, // intervalValue
                                                       1.0f),  // skewFactor
                                     0.2f),                    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Mix",
                                     "Mix",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     1.0f)                    // defaultValue
  );
}
} // namespace model
} // namespace dmt