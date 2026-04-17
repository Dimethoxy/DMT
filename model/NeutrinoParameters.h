#pragma once
//==============================================================================
#include "AhdEnvelopeParameters.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace model {
//==============================================================================
static inline juce::AudioProcessorParameterGroup
neutrinoParameterGroup(juce::String parentUid, [[maybe_unused]] int versionHint)
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  // using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  using ParameterGroup = juce::AudioProcessorParameterGroup;

  juce::String uid = parentUid + "Neutrino";

  return juce::AudioProcessorParameterGroup(
    uid,        // group ID
    "Neutrino", // group name
    "|",        // separator

    // Envelopes
    std::make_unique<ParameterGroup>(envelopeParameterGroup(uid,
                                                            "Gain",
                                                            {
                                                              0.0f,
                                                              0.04f,
                                                              0.335f,
                                                              0.0f,
                                                              2.0f,
                                                            })),
    std::make_unique<ParameterGroup>(envelopeParameterGroup(
      uid, "Pitch", { 0.0f, 0.0f, 0.144f, 1.0f, 7.5f })));
}
} // namespace model
} // namespace dmt