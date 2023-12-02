/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../gui/panels/GainPanel.h"
#include "../gui/panels/OscSendPanel.h"
#include "../gui/panels/OscillatorPanel.h"
#include "../gui/panels/PitchPanel.h"
#include "../gui/panels/VoicingPanel.h"
#include "../gui/panels/WaveformDistortionPanel.h"
#include "../utility/AppSettings.h"
#include "./PluginProcessor.h"
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
