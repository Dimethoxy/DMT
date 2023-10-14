/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "./Utility/AppSettings.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class NeutrinoAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  NeutrinoAudioProcessorEditor(NeutrinoAudioProcessor&);
  ~NeutrinoAudioProcessorEditor() override;

  //============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  NeutrinoAudioProcessor& audioProcessor;
  juce::MidiKeyboardComponent keyboardComponent;

  juce::Rectangle<int> testRect;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
