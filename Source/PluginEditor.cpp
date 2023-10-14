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
  , keyboardComponent(p.keyboardState,
                      juce::MidiKeyboardComponent::horizontalKeyboard)
{
  setSize(1805, 1160);
  setResizable(true, true);
}

NeutrinoAudioProcessorEditor::~NeutrinoAudioProcessorEditor() {}

//==============================================================================
void
NeutrinoAudioProcessorEditor::paint(juce::Graphics& g)
{
  g.fillAll(dmt::AppSettings::Colours::background);
  g.setColour(juce::Colours::azure);
  g.fillRect(testRect);
}

void
NeutrinoAudioProcessorEditor::resized()
{
  using Settings = dmt::AppSettings;
  float rawMargin = Settings::Layout::margin;
  auto bounds = getLocalBounds();
  float rawWidth = 1805.0f;
  float width = bounds.getWidth();
  float rawHeight = 1160.0f;
  float height = bounds.getHeight();
  float size = height / rawHeight;
  Settings::size = size;
  float margin = rawMargin * size;

  float rawLeftWidth = Settings::Layout::leftWidth + 2.0f * rawMargin;
  float leftWidth = rawLeftWidth * size;
  float rawCenterWidth = Settings::Layout::centerWidth + 2.0f * rawMargin;
  float centerWidth = rawCenterWidth * size;
  float rawRightWidth = Settings::Layout::rightWidth + 2.0f * rawMargin;
  float rightWidth = rawRightWidth * size;

  float rawHeaderHeight = Settings::Layout::headerHeight + 2.0f * rawMargin;
  float headerHeight = rawHeaderHeight * size;
  float rawTabHeight = 60.0f + 2.0f * rawMargin;
  float tabHeight = rawTabHeight * size;
  float rawRowHeight = 300.0f + 2.0f * rawMargin;
  float rowHeight = rawRowHeight * size;
  
  auto innerBounds = bounds.reduced(margin);
  auto headerBounds = innerBounds.removeFromTop(headerHeight);
  auto tabBounds = innerBounds.removeFromTop(tabHeight);
  auto topBounds = innerBounds.removeFromTop(rowHeight);
  auto midBounds = innerBounds.removeFromTop(rowHeight);
  auto bottomBounds = innerBounds.removeFromTop(rowHeight);

  innerBounds = bounds.reduced(margin);
  auto leftBounds = innerBounds.removeFromLeft(leftWidth);
  auto centerBounds = innerBounds.removeFromLeft(centerWidth);
  auto rightBounds = innerBounds.removeFromLeft(rightWidth);

  testRect = headerBounds.toNearestInt();
}
//==============================================================================
