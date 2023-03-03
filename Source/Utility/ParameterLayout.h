/*
  ==============================================================================

    ParameterLayout.h
    Created: 3 Mar 2023 3:51:03am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace dmt {
static juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout()
{
  return juce::AudioProcessorValueTreeState::ParameterLayout{
    std::make_unique<juce::AudioParameterFloat>("oscGain",
                                                "Osc Gain",
                                                juce::NormalisableRange<float>(
                                                  // rangeStart
                                                  -32.0f,
                                                  // rangeEnd
                                                  0.0f,
                                                  // intervalValue
                                                  0.2f,
                                                  // skewFactor
                                                  1.0f),
                                                // defaultValue
                                                0.0f),

    std::make_unique<juce::AudioParameterFloat>("oscDrive",
                                                "Osc Drive",
                                                juce::NormalisableRange<float>(
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

    std::make_unique<juce::AudioParameterFloat>("oscAmpAttack",
                                                "Amp Attack",
                                                juce::NormalisableRange<float>(
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
    std::make_unique<juce::AudioParameterFloat>("oscAmpHold",
                                                "Amp Hold",
                                                juce::NormalisableRange<float>(
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
    std::make_unique<juce::AudioParameterFloat>("oscAmpDecay",
                                                "Amp Decay",
                                                juce::NormalisableRange<float>(
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

    std::make_unique<juce::AudioParameterFloat>("oscModDecay",
                                                "Mod Decay",
                                                juce::NormalisableRange<float>(
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

    std::make_unique<juce::AudioParameterFloat>("oscModDepth",
                                                "Mod Depth",
                                                juce::NormalisableRange<float>(
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

    std::make_unique<juce::AudioParameterFloat>("oscModScew",
                                                "Mod Scew",
                                                juce::NormalisableRange<float>(
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
  };
}
}
