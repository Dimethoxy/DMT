/*
  ==============================================================================

                PresetPanel.h
                Created: 20 Dec 2022 10:29:23pm
                Author:  Lunix

                Source: https://youtu.be/YwAtWuGA4Cg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class PresetPanel
  : public juce::Component
  , juce::Button::Listener
  , juce::ComboBox::Listener
{
public:
  PresetPanel(dmt::PresetManager& pm)
    : presetManager(pm)
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

    loadPresetList();
  }

  ~PresetPanel() override
  {
    saveButton.removeListener(this);
    previousPresetButton.removeListener(this);
    nextPresetButton.removeListener(this);
    deleteButton.removeListener(this);
    presetList.removeListener(this);
  }

  void paint(juce::Graphics&) override {}

  void resized() override
  {
    auto padding = 4;
    const auto container = getLocalBounds().reduced(padding);
    auto bounds = container;

    saveButton.setBounds(
      bounds.removeFromLeft(container.proportionOfWidth(0.2)).reduced(padding));
    previousPresetButton.setBounds(
      bounds.removeFromLeft(container.proportionOfWidth(0.1)).reduced(padding));
    presetList.setBounds(
      bounds.removeFromLeft(container.proportionOfWidth(0.4)).reduced(padding));
    nextPresetButton.setBounds(
      bounds.removeFromLeft(container.proportionOfWidth(0.1)).reduced(padding));
    deleteButton.setBounds(bounds.reduced(padding));
  }

private:
  void buttonClicked(juce::Button* button) override
  {
    if (button == &saveButton) {
      fileChooser =
        std::make_unique<juce::FileChooser>("Save Preset",
                                            presetManager.defaultDirectory,
                                            "*." + presetManager.extension);

      fileChooser->launchAsync(juce::FileBrowserComponent::saveMode,
                               [&](const juce::FileChooser& chooser) {
                                 const auto result = chooser.getResult();
                                 presetManager.savePreset(result.getFileName());
                                 loadPresetList();
                               });
    }
    if (button == &previousPresetButton) {
      const auto index = presetManager.loadPreviousPreset();
      presetList.setSelectedItemIndex(index, juce::dontSendNotification);
    }
    if (button == &nextPresetButton) {
      const auto index = presetManager.loadNextPreset();
      presetList.setSelectedItemIndex(index, juce::dontSendNotification);
    }
    if (button == &deleteButton) {
      presetManager.deletePreset(presetManager.getCurrentPreset());
      loadPresetList();
    }
  }

  void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
  {
    if (comboBoxThatHasChanged == &presetList) {
      presetManager.loadPreset(
        presetList.getItemText(presetList.getSelectedItemIndex()));
    }
  }

  void configureButton(juce::Button& button, const juce::String& buttontext)
  {
    button.setButtonText(buttontext);
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(button);
    button.addListener(this);
  }

  void loadPresetList()
  {
    presetList.clear(juce::dontSendNotification);
    const auto allPresets = presetManager.getPresetList();
    const auto currentPreset = presetManager.getCurrentPreset();
    presetList.addItemList(allPresets, 1);
    presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset),
                                    juce::dontSendNotification);
  }

  dmt::PresetManager presetManager;
  std::unique_ptr<juce::FileChooser> fileChooser;
  juce::TextButton saveButton;
  juce::TextButton previousPresetButton;
  juce::TextButton nextPresetButton;
  juce::TextButton deleteButton;

  juce::ComboBox presetList;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};