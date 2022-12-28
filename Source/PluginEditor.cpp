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
{
  addAndMakeVisible(folderPanel);
  addAndMakeVisible(presetPanel);
  addAndMakeVisible(genericAudioProcessorEditor);
  setSize(500, 500);
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

  genericAudioProcessorEditor.setBoundsRelative(0.0f, 0.15f, 1.0f, 0.8f);
  folderPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.06)));
  presetPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.06)));
}
