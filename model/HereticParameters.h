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
    std::make_unique<ParameterFloat>(uid + "PreGain",
                                     "PreGain",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     1.0f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Spread",
                                     "Spread",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.5f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Range",
                                     "Range",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       16.0f, // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     8.0f),                   // defaultValue
    // defaultValue
    std::make_unique<ParameterFloat>(uid + "Mix",
                                     "Mix",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.5f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Distortion",
                                     "Distortion",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.1f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Drive",
                                     "Drive",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.5f),                   // defaultValue
    std::make_unique<AudioParameterChoice>(uid + "DriveType",
                                           "Drive Type",
                                           juce::StringArray{ "Hard Clipping",
                                                              "Soft Clipping",
                                                              "Saturate",
                                                              "Atan",
                                                              "Crunch",
                                                              "Bitcrush",
                                                              "Extreme",
                                                              "Screame",
                                                              "Sine",
                                                              "Cosine",
                                                              "Harmonic",
                                                              "Weird" },
                                           0), // defaultItemIndex
    std::make_unique<ParameterFloat>(uid + "DriveBias",
                                     "Drive Bias",
                                     NormalisableRange(-1.0f, // rangeStart
                                                       1.0f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.0f),                   // defaultValue
    std::make_unique<ParameterFloat>(uid + "Feedback",
                                     "Feedback",
                                     NormalisableRange(0.0f,  // rangeStart
                                                       0.9f,  // rangeEnd
                                                       0.01f, // intervalValue
                                                       1.0f), // skewFactor
                                     0.0f),                   // defaultValue
    std::make_unique<AudioParameterChoice>(uid + "FeedbackFilterSlope",
                                           "Feedback Filter Slope",
                                           juce::StringArray{ "LP 12dB/Oct",
                                                              "LP 24dB/Oct",
                                                              "LP 36dB/Oct",
                                                              "LP 48dB/Oct",
                                                              "BP 12dB/Oct",
                                                              "BP 24dB/Oct",
                                                              "BP 36dB/Oct",
                                                              "BP 48dB/Oct",
                                                              "HP 12dB/Oct",
                                                              "HP 24dB/Oct",
                                                              "HP 36dB/Oct",
                                                              "HP 48dB/Oct" },
                                           0), // defaultItemIndex
    std::make_unique<ParameterFloat>(uid + "FeedbackFilterCutoff",
                                     "Feedback Filter Cutoff",
                                     NormalisableRange(20.0f,    // rangeStart
                                                       20000.0f, // rangeEnd
                                                       1.0f,   // intervalValue
                                                       0.25f), // skewFactor
                                     20000.0f));
}
} // namespace model
} // namespace dmt