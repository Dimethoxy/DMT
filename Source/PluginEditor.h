/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "./Preset/FolderManager.h"
#include "./Preset/FolderPanel.h"
#include "./Preset/PresetPanel.h"
#include "./Utility/Appsettings.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

namespace dmt {
struct Shadow : public juce::DropShadow
{
      void drawInnerForPath(juce::Graphics& g, juce::Path target)
      {
        juce::Graphics::ScopedSaveState saveState(g);
        juce::Path shadowPath(target);
        shadowPath.addRectangle(target.getBounds().expanded(10));
        shadowPath.setUsingNonZeroWinding(false);
        g.reduceClipRegion(target);
        juce::DropShadow ds(colour, radius, offset);
        ds.drawForPath(g, shadowPath);
      }
};
class Panel : public juce::Component
{
public:
  Panel()
  {
    dropShadow.radius = 5.0f;
    dropShadow.colour = AppSettings::Colours::panelShadow;
  }
  void paint(juce::Graphics& g)
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(AppSettings::Colours::background);
    g.fillRect(bounds);

    const auto borderSize = AppSettings::Panel::borderSize;
    const auto borderBounds = bounds.reduced(AppSettings::Panel::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = AppSettings::Panel::outerCornerSize;
    const auto innerCornerSize = AppSettings::Panel::innerCornerSize;

    juce::Path shadowPath;
    shadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
    dropShadow.drawForPath(g, shadowPath);

    g.setColour(AppSettings::Colours::panelBorder);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(AppSettings::Colours::panelBackground);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    juce::Path innerShadowPath;
    innerShadowPath.addRoundedRectangle(innerBounds,
                                        outerCornerSize);
    dropShadow.drawInnerForPath(g, innerShadowPath);
  }

private:
  dmt::Shadow dropShadow;
};
}

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
  juce::MidiKeyboardComponent keyboardComponent;
  dmt::PresetPanel presetPanel;
  dmt::FolderPanel folderPanel;
  dmt::Panel oscPannel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
