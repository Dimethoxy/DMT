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

  // Version hints for safe parameter addition in AudioUnit hosts
  constexpr int versionHint = 10100; // 01 01 00 = 1.1.0

  return juce::AudioProcessorParameterGroup(
    uid,       // group ID
    "Disflux", // group name
    "|",       // separator
    std::make_unique<ParameterFloat>(
      uid + "Amount",
      "Amount",
      NormalisableRange(1.0f,   // rangeStart
                        128.0f, // rangeEnd
                        1.0f,   // intervalValue
                        0.5f),  // skewFactor
      1.0f,                     // defaultValue
      juce::String(),           // label
      juce::AudioProcessorParameter::genericParameter,
      nullptr,
      nullptr,
      versionHint), // versionHint
    std::make_unique<ParameterFloat>(
      uid + "Spread",
      "Spread",
      NormalisableRange(0.0f,     // rangeStart
                        10000.0f, // rangeEnd
                        1.0f,     // intervalValue
                        0.5f),    // skewFactor
      200.0f,                     // defaultValue
      juce::String(),
      juce::AudioProcessorParameter::genericParameter,
      nullptr,
      nullptr,
      versionHint),
    std::make_unique<ParameterFloat>(
      uid + "Frequency",
      "Frequency",
      NormalisableRange(20.0f,    // rangeStart
                        20000.0f, // rangeEnd
                        1.0f,     // intervalValue
                        0.25f),   // skewFactor
      50.0f,                      // defaultValue
      juce::String(),
      juce::AudioProcessorParameter::genericParameter,
      nullptr,
      nullptr,
      versionHint),
    std::make_unique<ParameterFloat>(
      uid + "Pinch",
      "Pinch",
      NormalisableRange(0.5f,   // rangeStart
                        16.0f,  // rangeEnd
                        0.001f, // intervalValue
                        1.0f),  // skewFactor
      1.0f,                     // defaultValue
      juce::String(),
      juce::AudioProcessorParameter::genericParameter,
      nullptr,
      nullptr,
      versionHint),
    std::make_unique<ParameterFloat>(
      uid + "Mix",
      "Mix",
      NormalisableRange(0.0f,  // rangeStart
                        1.0f,  // rangeEnd
                        0.01f, // intervalValue
                        1.0f), // skewFactor
      1.0f,                    // defaultValue
      juce::String(),
      juce::AudioProcessorParameter::genericParameter,
      nullptr,
      nullptr,
      versionHint));
}
} // namespace model
} // namespace dmt