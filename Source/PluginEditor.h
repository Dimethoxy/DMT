/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Preset/FolderManager.h"
#include "./Preset/FolderPanel.h"
#include "./Preset/PresetPanel.h"
#include "./Utility/Appsettings.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

namespace dmt {
class OscillatorEditor : juce::Component
{
public:
  OscillatorEditor(dmt::AppSettings& a)
    : a(a)
  {
  }
  void paint(juce::Graphics& g)
  {
    g.setColour(dmt::AppSettings::Colours::solidLight);
    g.fillRect(this->getLocalBounds());

    g.setColour(a.colours.solidMedium);
    g.fillRect(this->getLocalBounds().reduced(a.margin * a.size / 3.0f));
    g.setColour(a.colours.solidDark);
    g.fillRect(this->getLocalBounds().reduced(a.margin * a.size / 2.0f));
  }

private:
  dmt::AppSettings& a;
};
}

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
  dmt::PresetPanel presetPanel;
  dmt::FolderPanel folderPanel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
