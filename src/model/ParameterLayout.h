//==============================================================================

#pragma once

#include "../dsp/filter/FilterProcessor.h"
#include "OscillatorParameters.h"
#include "VoiceParameters.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
static inline juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout()
{
  using ParameterInt = juce::AudioParameterInt;
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using ParameterGroup = juce::AudioProcessorParameterGroup;
  using NormalisableRange = juce::NormalisableRange<float>;
  return juce::AudioProcessorValueTreeState::ParameterLayout{
    std::make_unique<ParameterGroup>(dmt::model::voiceParameterGroup()),
    std::make_unique<ParameterGroup>(dmt::model::oscillatorParameterGroup()),

    //============================================================================
    std::make_unique<ParameterFloat>("oscAmpAttack",
                                     "Amp Attack",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       1.0f,
                                       // intervalValue
                                       0.001f,
                                       // skewFactor
                                       0.5f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscAmpHold",
                                     "Amp Hold",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       1.0f,
                                       // intervalValue
                                       0.001f,
                                       // skewFactor
                                       0.5f),
                                     // defaultValue
                                     0.08f),
    std::make_unique<ParameterFloat>("oscAmpDecay",
                                     "Amp Decay",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       3.0f,
                                       // intervalValue
                                       0.001f,
                                       // skewFactor
                                       0.5f),
                                     // defaultValue
                                     0.5f),
    //============================================================================
    std::make_unique<ParameterFloat>("oscModDecay",
                                     "Mod Decay",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       1.0f,
                                       // intervalValue
                                       0.001f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.3f),
    std::make_unique<ParameterFloat>("oscModDepth",
                                     "Mod Depth",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       20000.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     20000.0f),
    std::make_unique<ParameterFloat>("oscModScew",
                                     "Mod Scew",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       32.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     32.0f),
    //============================================================================
    std::make_unique<ParameterChoice>(
      "filterType", "Filter", dmt::dsp::filter::IIRFilterState::typeNames, 0),
    std::make_unique<ParameterFloat>("filterCutoff",
                                     "Filter Cutoff",
                                     NormalisableRange(
                                       // rangeStart
                                       20.0f,
                                       // rangeEnd
                                       20000.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       0.25f),
                                     // defaultValue
                                     425.0f),
    std::make_unique<ParameterFloat>("filterQ",
                                     "Filter Q",
                                     NormalisableRange(
                                       // rangeStart
                                       0.1f,
                                       // rangeEnd
                                       10.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     1.0f),
    std::make_unique<ParameterFloat>("filterGain",
                                     "Filter Gain",
                                     NormalisableRange(
                                       // rangeStart
                                       -32.0f,
                                       // rangeEnd
                                       32.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    //============================================================================
  };
}
} // namespace dmt
