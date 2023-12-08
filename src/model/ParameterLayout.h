//==============================================================================

#pragma once

#include "../dsp/filter/FilterProcessor.h"

#include "OscillatorParameters.h"

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
  namespace Model = dmt::model;
  return juce::AudioProcessorValueTreeState::ParameterLayout{
    std::make_unique<ParameterGroup>(Model::oscillatorParameterGroup(1)),
    std::make_unique<ParameterGroup>(Model::oscillatorParameterGroup(2)),
    std::make_unique<ParameterGroup>(Model::oscillatorParameterGroup(3)),
    std::make_unique<ParameterGroup>(Model::oscillatorParameterGroup(4)),
    std::make_unique<ParameterGroup>(Model::oscillatorParameterGroup(5))
  };
}
} // namespace dmt
