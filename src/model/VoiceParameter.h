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
    "voiceParameters",  // group ID
    "Voice Parameters", // group name
    "|",                // separator
    // parameter list
    std::make_unique<ParameterInt>("osctave",                  // parameter ID
                                   "Osctave",                  // parameter name
                                   NormalisableRange(-4,       // rangeStart
                                                     4,        // rangeEnd
                                                     1,        // intervalValue
                                                     1),       // skewFactor
                                   0),                         // defaultValue
    std::make_unique<ParameterInt>("semitones",                // parameter ID
                                   "Semitones",                // parameter name
                                   NormalisableRange(0,        // rangeStart
                                                     12,       // rangeEnd
                                                     1,        // intervalValue
                                                     1),       // skewFactor
                                   0),                         // defaultValue
    std::make_unique<ParameterFloat>("fine",                   // parameter ID
                                     "Fine",                   // parameter name
                                     NormalisableRange(-100.f, // rangeStart
                                                       100.f,  // rangeEnd
                                                       0.1,    // intervalValue
                                                       1),     // skewFactor
                                     0),
    std::make_unique<ParameterInt>("density",                 // parameter ID
                                   "Density",                 // parameter name
                                   NormalisableRange(1,       // rangeStart
                                                     8,       // rangeEnd
                                                     1,       // intervalValue
                                                     1),      // skewFactor
                                   1),                        // defaultValue
    std::make_unique<ParameterFloat>("detune",                // parameter ID
                                     "Detune",                // parameter name
                                     NormalisableRange(0.f,   // rangeStart
                                                       100.f, // rangeEnd
                                                       0.1,   // intervalValue
                                                       1),    // skewFactor
                                     0),
    std::make_unique<ParameterChoice>("distribution", // parameter ID
                                      "Distribution", // parameter name
                                      juce::StringArray{ "Linear",
                                                         "Quadratic",
                                                         "Cubic",
                                                         "Octic",
                                                         "Square Root",
                                                         "Cube Root",
                                                         "Octic Root",
                                                         "Sine"
                                                         "Random" }, // choices
                                      0) // default index );
  );
}
} // namespace model
} // namespace dmt