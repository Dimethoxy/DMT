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
struct Shadow
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
  void drawOuterForPath(juce::Graphics& g, juce::Path target)
  {
    juce::Graphics::ScopedSaveState saveState(g);
    juce::Path shadowPath(target);
    shadowPath.addRectangle(target.getBounds().expanded(10));
    shadowPath.setUsingNonZeroWinding(false);
    g.reduceClipRegion(shadowPath);
    juce::DropShadow ds(colour, radius, offset);
    ds.drawForPath(g, target);
  }
  juce::Colour colour = juce::Colours::black;
  juce::Point<int> offset = { 0, 0 };
  int radius = 10;
};
class Panel : public juce::Component
{
public:
  using Settings = dmt::AppSettings::Panel;
  Panel()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(Settings::backgroundColour);
    g.fillRect(bounds);

    const auto borderSize = Settings::borderSize;
    const auto borderBounds = bounds.reduced(Settings::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = Settings::outerCornerSize;
    const auto innerCornerSize = Settings::innerCornerSize;

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    g.setColour(Settings::borderColour);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(Settings::foregroundColour);
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
class OscillatorDisplayComponent : public juce::Component
{
  using Settings = dmt::AppSettings::OscillatorDisplay;

public:
  OscillatorDisplayComponent()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    g.setColour(Settings::backgroundColour);
    g.fillRect(bounds);

    const auto borderSize = Settings::borderSize;
    const auto borderBounds = bounds.reduced(Settings::margin);
    const auto innerBounds = borderBounds.reduced(borderSize);
    const auto outerCornerSize = Settings::outerCornerSize;
    const auto innerCornerSize = Settings::innerCornerSize;

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    g.setColour(Settings::borderColour);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(Settings::foregroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, outerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }
  juce::Path getPath()
  {
    juce::Path path;

    auto startX = 0.0f;
    auto startY = getHeight() / 2.0f;
    juce::Point<float> start(startX, startY);

    auto endX = getWidth();
    auto endY = getHeight() / 2.0f;
    juce::Point<float> end(endX, endY);

    return path;
  }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel() { addAndMakeVisible(oscDisplay); }
  void resized() override
  {
    oscDisplay.setBoundsRelative(0.2f, 0.15f, 0.6f, 0.2f);
  }

private:
  dmt::OscillatorDisplayComponent oscDisplay;
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
  dmt::OscillatorPanel oscPannel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessorEditor)
};
