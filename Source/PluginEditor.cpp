/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeutrinoAudioProcessorEditor::NeutrinoAudioProcessorEditor(NeutrinoAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p),
	genericAudioProcessorEditor(p)
{
	addAndMakeVisible(presetPanel);
	addAndMakeVisible(genericAudioProcessorEditor);
	setSize(400, 400);
}

NeutrinoAudioProcessorEditor::~NeutrinoAudioProcessorEditor()
{
}

//==============================================================================
void NeutrinoAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NeutrinoAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();

	genericAudioProcessorEditor.setBoundsRelative(0.0f, 0.2f, 1.0f, 0.8f);
	presetPanel.setBounds(bounds.removeFromTop(proportionOfHeight(0.1)));
}
