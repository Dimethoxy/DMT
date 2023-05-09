/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
NeutrinoAudioProcessorEditor::NeutrinoAudioProcessorEditor(
  NeutrinoAudioProcessor& p)
  : AudioProcessorEditor(&p)
  , audioProcessor(p)
  , presetPanel(p.getPresetManager())
  , genericAudioProcessorEditor(p)
  , keyboardComponent(p.keyboardState,
                      juce::MidiKeyboardComponent::horizontalKeyboard)
  , oscPannel(p.apvts)
{
  addAndMakeVisible(folderPanel);
  addAndMakeVisible(presetPanel);
  addAndMakeVisible(genericAudioProcessorEditor);
  addAndMakeVisible(keyboardComponent);
  addAndMakeVisible(voicePannel);
  addAndMakeVisible(oscPannel);

  keyboardComponent.setLowestVisibleKey(0);
  keyboardComponent.setKeyWidth(21.5f);

  setSize(1600, 900);
}

NeutrinoAudioProcessorEditor::~NeutrinoAudioProcessorEditor() {}

//==============================================================================
void
NeutrinoAudioProcessorEditor::paint(juce::Graphics& g)
{
  g.fillAll(dmt::AppSettings::Colours::background);
}

void
NeutrinoAudioProcessorEditor::resized()
{
  dmt::AppSettings::size = getHeight() / 900.0f;
  auto bounds = getLocalBounds();

  genericAudioProcessorEditor.setBoundsRelative(0.7f, 0.075f, 0.3f, 0.75f);
  voicePannel.setBoundsRelative(0.0f, 0.075f, 0.25f, 0.35f);
  oscPannel.setBoundsRelative(0.0f, 0.40f, 0.25f, 0.4f);
  // folderPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.04)));
  presetPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.05f)));
  keyboardComponent.setBoundsRelative(0.0f, 0.85f, 1.0f, 0.15f);
}
