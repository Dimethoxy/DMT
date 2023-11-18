/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Utility/AppSettings.h"
#include "Gui/Panels/OscSendPanel.h"
#include "Gui/Panels/OscillatorPanel.h"
#include "Gui/Panels/VoicingPanel.h"
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

  //============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  NeutrinoAudioProcessor& audioProcessor;
  juce::MidiKeyboardComponent keyboardComponent;

  dmt::gui::panels::VoicingPanel voicingPanel;

  dmt::gui::panels::OscillatorPanel oscillatorPanel;
  dmt::gui::panels::OscillatorPanel gainPanel;
  dmt::gui::panels::OscillatorPanel pitchPanel;

  dmt::gui::panels::OscSendPanel sendPanelA;
  dmt::gui::panels::OscSendPanel sendPanelB;
  dmt::gui::panels::OscSendPanel sendPanelC;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
