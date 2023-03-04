//==============================================================================

#pragma once

#include "../Synth/AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
static juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout()
{
  using ParameterFloat = juce::AudioParameterFloat;
  using ParameterChoice = juce::AudioParameterChoice;
  using NormalisableRange = juce::NormalisableRange<float>;
  return juce::AudioProcessorValueTreeState::ParameterLayout{
    //==========================================================================
    std::make_unique<ParameterChoice>(
      "waveformType", "Waveform", dmt::AnalogWaveform::waveformNames, 2),
    std::make_unique<ParameterFloat>("oscGain",
                                     "Osc Gain",
                                     NormalisableRange(
                                       // rangeStart
                                       -96.0f,
                                       // rangeEnd
                                       0.0f,
                                       // intervalValue
                                       0.2f,
                                       // skewFactor
                                       2.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscDrive",
                                     "Osc Drive",
                                     NormalisableRange(
                                       // rangeStart
                                       1.0f,
                                       // rangeEnd
                                       10.0f,
                                       // intervalValue
                                       0.01f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     4.0f),
    std::make_unique<ParameterFloat>("oscBias",
                                     "Osc Bias",
                                     NormalisableRange(
                                       // rangeStart
                                       -1.0f,
                                       // rangeEnd
                                       1.0f,
                                       // intervalValue
                                       0.01f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
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
    //============================================================================
  };
}
}
