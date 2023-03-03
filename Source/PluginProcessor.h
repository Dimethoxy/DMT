/*
 ==============================================================================

 This file contains the basic framework code for a JUCE plugin
 processor.

 ==============================================================================
 */

#pragma once

#include "./Preset/PresetManager.h"
#include "./Synth/SynthSound.h"
#include "./Synth/SynthVoice.h"
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

//==============================================================================
class NeutrinoAudioProcessor
  : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
  , public juce::AudioProcessorARAExtension
#endif
{
public:
  //==============================================================================
  NeutrinoAudioProcessor();
  ~NeutrinoAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //==============================================================================
  dmt::PresetManager& getPresetManager() { return *presetManager; }

private:
  //==============================================================================
  juce::AudioProcessorValueTreeState valueTreeState;
  std::unique_ptr<dmt::PresetManager> presetManager;

  juce::Synthesiser synth;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessor)
};
