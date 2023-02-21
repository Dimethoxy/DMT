/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Preset/FolderManager.h"
#include "./Preset/FolderPanel.h"
#include "./Preset/PresetPanel.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class NeutrinoAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  NeutrinoAudioProcessorEditor(NeutrinoAudioProcessor&);
  ~NeutrinoAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  NeutrinoAudioProcessor& audioProcessor;
  juce::GenericAudioProcessorEditor genericAudioProcessorEditor;
  dmt::PresetPanel presetPanel;
  dmt::FolderPanel folderPanel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
