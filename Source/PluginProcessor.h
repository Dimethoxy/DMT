/*
 ==============================================================================

 This file contains the basic framework code for a JUCE plugin
 processor.

 ==============================================================================
 */

#pragma once

#include "./Filter/FilterProcessor.h"
#include "./Preset/PresetManager.h"
#include "./Synth/SynthSound.h"
#include "./Synth/SynthVoice.h"
#include "./Utility/ChainSettings.h"
#include "./Utility/ParameterLayout.h"
#include <JuceHeader.h>

//==============================================================================
class NeutrinoAudioProcessor
  : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
  , public juce::AudioProcessorARAExtension
#endif
{
public:
  //============================================================================
  NeutrinoAudioProcessor();
  ~NeutrinoAudioProcessor() override;

  //============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  //============================================================================
  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  //============================================================================
  dmt::PresetManager& getPresetManager() { return *presetManager; }
  juce::MidiKeyboardState keyboardState;

private:
  //============================================================================
  juce::AudioProcessorValueTreeState apvts;
  std::unique_ptr<dmt::PresetManager> presetManager;

  juce::Synthesiser synth;

  dmt::FilterProcessor filterProcessor;

  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessor)
};
