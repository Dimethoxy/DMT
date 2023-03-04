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
  ,

  audioProcessor(p)
  , presetPanel(p.getPresetManager())
  , genericAudioProcessorEditor(p)
  , keyboardComponent(p.keyboardState,
                      juce::MidiKeyboardComponent::horizontalKeyboard)
{
  addAndMakeVisible(folderPanel);
  addAndMakeVisible(presetPanel);
  addAndMakeVisible(genericAudioProcessorEditor);
  addAndMakeVisible(keyboardComponent);
  keyboardComponent.setLowestVisibleKey(34);
  setSize(500, 700);
}

NeutrinoAudioProcessorEditor::~NeutrinoAudioProcessorEditor() {}

//==============================================================================
void
NeutrinoAudioProcessorEditor::paint(juce::Graphics& g)
{
  g.fillAll(
    getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void
NeutrinoAudioProcessorEditor::resized()
{
  auto bounds = getLocalBounds();

  genericAudioProcessorEditor.setBoundsRelative(0.0f, 0.075f, 1.0f, 0.75f);
  // folderPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.04)));
  presetPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.05f)));
  keyboardComponent.setBoundsRelative(0.0f, 0.9f, 1.0f, 0.1f);
}
