/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Gui/Panels/OscillatorPanel.h"
#include "./Gui/Panels/VoicePanel.h"
#include "./Gui/Preset/FolderManager.h"
#include "./Gui/Preset/FolderPanel.h"
#include "./Gui/Preset/PresetPanel.h"
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
  juce::MidiKeyboardComponent keyboardComponent;
  dmt::gui::preset::PresetPanel presetPanel;
  dmt::gui::preset::FolderPanel folderPanel;
  dmt::gui::panels::VoicePanel voicePannel;
  dmt::gui::panels::OscillatorPanel oscPannel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
