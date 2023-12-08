#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace model {
static inline juce::AudioProcessorParameterGroup
voiceParameterGroup()
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  return juce::AudioProcessorParameterGroup(
    "VoiceParameters",                                         // group ID
    "Voice Parameters",                                        // group name
    "|",                                                       // separator
    std::make_unique<ParameterInt>("osctave",                  // parameter ID
                                   "Osctave",                  // parameter name
                                   -4,                         // rangeStart
                                   4,                          // rangeEnd
                                   0),                         // defaultValue
    std::make_unique<ParameterInt>("semitones",                // parameter ID
                                   "Semitones",                // parameter name
                                   0,                          // rangeStart
                                   12,                         // rangeEnd
                                   0),                         // defaultValue
    std::make_unique<ParameterFloat>("fine",                   // parameter ID
                                     "Fine",                   // parameter name
                                     NormalisableRange(-100.f, // rangeStart
                                                       100.f,  // rangeEnd
                                                       .1f,    // intervalValue
                                                       1.f),   // skewFactor
                                     0),
    std::make_unique<ParameterInt>("density",         // parameter ID
                                   "Density",         // parameter name
                                   1,                 // rangeStart
                                   8,                 // rangeEnd
                                   1),                // defaultValue
    std::make_unique<ParameterChoice>("distribution", // parameter ID
                                      "Distribution", // parameter name
                                      juce::StringArray{ "Linear",
                                                         "Quadratic",
                                                         "Cubic",
                                                         "Octic",
                                                         "Square Root",
                                                         "Cube Root",
                                                         "Octic Root",
                                                         "Sine",
                                                         "Random" }, // choices
                                      0),                     // default index
    std::make_unique<ParameterFloat>("blend",                 // parameter ID
                                     "Blend",                 // parameter name
                                     NormalisableRange(0.f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .1f,   // intervalValue
                                                       1.f),  // skewFactor
                                     0.f),                    // defaultValue
    std::make_unique<ParameterFloat>("width",                 // parameter ID
                                     "Width",                 // parameter name
                                     NormalisableRange(0.f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       .1f,   // intervalValue
                                                       1.f),  // skewFactor
                                     0.f),                    // defaultValue
    std::make_unique<ParameterChoice>(
      "seed", // parameter ID
      "Seed", // parameter name
      juce::StringArray{ "Random",
                         "Equal",
                         "Static #1",
                         "Static #2",
                         "Static #3",
                         "Static #4",
                         "Static #5" }, // choices
      0)                                // default index
  );
}
} // namespace model
} // namespace dmt