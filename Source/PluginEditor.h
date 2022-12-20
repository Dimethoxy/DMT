/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"

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
	PresetPanel presetPanel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
