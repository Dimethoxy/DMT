//==============================================================================

#pragma once

#include "../dsp/filter/FilterProcessor.h"
#include "../dsp/synth/AnalogWaveform.h"
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
      "waveformType",
      "Waveform",
      dmt::dsp::synth::AnalogWaveform::waveformNames,
      2),
    std::make_unique<ParameterFloat>("oscGain",
                                     "Osc Gain",
                                     NormalisableRange(
                                       // rangeStart
                                       -96.0f,
                                       // rangeEnd
                                       0.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscDrive",
                                     "Osc Drive",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
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
    std::make_unique<ParameterFloat>("oscBend",
                                     "Osc Bend",
                                     NormalisableRange(
                                       // rangeStart
                                       -100.0f,
                                       // rangeEnd
                                       100.0f,
                                       // intervalValue
                                       0.1f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscPwm",
                                     "Osc PWM",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       100.0f,
                                       // intervalValue
                                       0.01f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscSync",
                                     "Osc Sync",
                                     NormalisableRange(
                                       // rangeStart
                                       0.0f,
                                       // rangeEnd
                                       100.0f,
                                       // intervalValue
                                       0.01f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    //============================================================================
    std::make_unique<ParameterFloat>("oscOctave",
                                     "Octave",
                                     NormalisableRange(
                                       // rangeStart
                                       -4.0f,
                                       // rangeEnd
                                       +4.0f,
                                       // intervalValue
                                       1.0f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscSemitone",
                                     "Semitone",
                                     NormalisableRange(
                                       // rangeStart
                                       -24.0f,
                                       // rangeEnd
                                       +24.0f,
                                       // intervalValue
                                       1.0f,
                                       // skewFactor
                                       1.0f),
                                     // defaultValue
                                     0.0f),
    std::make_unique<ParameterFloat>("oscFine",
                                     "Fine",
                                     NormalisableRange(
                                       // rangeStart
                                       -100.0f,
                                       // rangeEnd
                                       +100.0f,
                                       // intervalValue
                                       1.0f,
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
