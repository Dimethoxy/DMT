/*
  ==============================================================================

	PresetPanel.h
	Created: 20 Dec 2022 10:29:23pm
	Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
{
public:
	PresetPanel()
	{
		configureButton(saveButton, "Save");
		configureButton(previousPresetButton, "<");
		configureButton(nextPresetButton, ">");
		configureButton(deleteButton, "Delete");

		presetList.setTextWhenNothingSelected("No Preset Selected");
		presetList.setTextWhenNoChoicesAvailable("No Presets Available");
		presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
		addAndMakeVisible(presetList);
		presetList.addListener(this);
	}

	~PresetPanel() override
	{
		saveButton.removeListener(this);
		previousPresetButton.removeListener(this);
		nextPresetButton.removeListener(this);
		deleteButton.removeListener(this);
		presetList.removeListener(this);
	}

	//==============================================================================
	void paint(juce::Graphics&) override
	{

	}

	void resized() override
	{
		auto padding = 4;
		const auto container = getLocalBounds().reduced(padding);
		auto bounds = container;

		saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.2)).reduced(padding));
		previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1)).reduced(padding));
		presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4)).reduced(padding));
		nextPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.1)).reduced(padding));
		deleteButton.setBounds(bounds.reduced(padding));
	}

private:
	void buttonClicked(juce::Button* button) override
	{

	}

	void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
	{

	}

	void configureButton(juce::Button& button, const juce::String& buttontext)
	{
		button.setButtonText(buttontext);
		button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
		addAndMakeVisible(button);
		button.addListener(this);
	}

	//==============================================================================
	juce::TextButton saveButton;
	juce::TextButton previousPresetButton;
	juce::TextButton nextPresetButton;
	juce::TextButton deleteButton;

	juce::ComboBox presetList;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};