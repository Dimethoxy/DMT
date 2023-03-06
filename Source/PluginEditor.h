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
  using Settings = dmt::AppSettings::Panel;
  Panel()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    innerShadow.colour = Settings::outerShadowColour;
    outerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  void paint(juce::Graphics& g)
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(AppSettings::Colours::background);
    g.fillRect(bounds);

    const auto borderSize = Settings::borderSize;
    const auto borderBounds = bounds.reduced(Settings::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = Settings::outerCornerSize;
    const auto innerCornerSize = Settings::innerCornerSize;

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
      outerShadow.drawForPath(g, outerShadowPath);
    }

    g.setColour(Settings::borderColour);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(Settings::backgroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, outerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
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
