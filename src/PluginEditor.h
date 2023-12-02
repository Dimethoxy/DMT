/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Utility/AppSettings.h"
#include "Gui/Panels/GainPanel.h"
#include "Gui/Panels/OscSendPanel.h"
#include "Gui/Panels/OscillatorPanel.h"
#include "Gui/Panels/PitchPanel.h"
#include "Gui/Panels/VoicingPanel.h"
#include "Gui/Panels/WaveformDistortionPanel.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

namespace Panels = dmt::gui::panels;

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

  Panels::OscillatorPanel oscillatorPanel;
  Panels::VoicingPanel voicingPanel;

  Panels::GainPanel gainPanel;
  Panels::PitchPanel pitchPanel;
  Panels::WaveformDistortionPanel distortionPanel;

  Panels::OscSendPanel sendPanelA;
  Panels::OscSendPanel sendPanelB;
  Panels::OscSendPanel sendPanelC;

  OpenGLContext openGLContext;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
