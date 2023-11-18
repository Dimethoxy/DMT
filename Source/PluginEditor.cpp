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
  , sendPanelA("A")
  , sendPanelB("B")
  , sendPanelC("C")
  , keyboardComponent(p.keyboardState,
                      juce::MidiKeyboardComponent::horizontalKeyboard)
{
  using Settings = dmt::AppSettings;
  using Layout = dmt::AppSettings::Layout;
  const float& leftWidth = Layout::leftWidth;
  const float& centerWidth = Layout::centerWidth;
  const float& rightWidth = Layout::rightWidth;
  addAndMakeVisible(voicingPanel);

  addAndMakeVisible(oscillatorPanel);
  addAndMakeVisible(gainPanel);
  addAndMakeVisible(pitchPanel);

  addAndMakeVisible(sendPanelA);
  addAndMakeVisible(sendPanelB);
  addAndMakeVisible(sendPanelC);

  addAndMakeVisible(keyboardComponent);

  setSize(Layout::getWidth(), Layout::getHeight());
  setResizable(true, true);
}

NeutrinoAudioProcessorEditor::~NeutrinoAudioProcessorEditor() {}

//==============================================================================
void
NeutrinoAudioProcessorEditor::paint(juce::Graphics& g)
{
  g.fillAll(dmt::AppSettings::Colours::background);
}

void
NeutrinoAudioProcessorEditor::resized()
{
  using Settings = dmt::AppSettings;
  using Layout = dmt::AppSettings::Layout;
  float& size = Layout::size;

  const float rawMargin = Settings::Layout::margin;
  const auto bounds = getLocalBounds();
  const float width = bounds.getWidth();
  const float height = bounds.getHeight();
  size = height / Layout::getHeight();
  const float margin = rawMargin * size;

  const float rawLeftWidth = Settings::Layout::leftWidth + 2.0f * margin;
  const float leftWidth = rawLeftWidth * size;
  const float rawCenterWidth = Settings::Layout::centerWidth + 2.0f * margin;
  const float centerWidth = rawCenterWidth * size;
  const float rawRightWidth = Settings::Layout::rightWidth + 2.0f * margin;
  const float rightWidth = rawRightWidth * size;

  const float rawHeaderHeight = Settings::Layout::headerHeight + 2.0f * margin;
  const float headerHeight = rawHeaderHeight * size;
  const float rawTabHeight = 60.0f + 2.0f * margin;
  const float tabHeight = rawTabHeight * size;
  const float rawRowHeight = 300.0f + 2.0f * margin;
  const float rowHeight = rawRowHeight * size;

  auto innerBounds = bounds.reduced(margin);
  const auto headerBounds = innerBounds.removeFromTop(headerHeight);
  const auto tabBounds = innerBounds.removeFromTop(tabHeight);
  const auto topBounds = innerBounds.removeFromTop(rowHeight);
  const auto midBounds = innerBounds.removeFromTop(rowHeight);
  const auto bottomBounds = innerBounds.removeFromTop(rowHeight);

  innerBounds = bounds.reduced(margin);
  const auto leftBounds = innerBounds.removeFromLeft(leftWidth);
  const auto centerBounds = innerBounds.removeFromLeft(centerWidth);
  const auto rightBounds = innerBounds.removeFromLeft(rightWidth);

  oscillatorPanel.setBounds(leftBounds.getX(),
                            topBounds.getY(),
                            leftBounds.getWidth(),
                            topBounds.getHeight() + bottomBounds.getHeight());
  gainPanel.setBounds(centerBounds.getX(),
                      topBounds.getY(),
                      centerBounds.getWidth(),
                      topBounds.getHeight());
  pitchPanel.setBounds(centerBounds.getX(),
                       midBounds.getY(),
                       centerBounds.getWidth(),
                       midBounds.getHeight());
  voicingPanel.setBounds(leftBounds.getX(),
                         bottomBounds.getY(),
                         leftBounds.getWidth(),
                         bottomBounds.getHeight());
  sendPanelA.setBounds(rightBounds.getX(),
                       topBounds.getY(),
                       rightBounds.getWidth(),
                       topBounds.getHeight());
  sendPanelB.setBounds(rightBounds.getX(),
                       midBounds.getY(),
                       rightBounds.getWidth(),
                       midBounds.getHeight());
  sendPanelC.setBounds(rightBounds.getX(),
                       bottomBounds.getY(),
                       rightBounds.getWidth(),
                       bottomBounds.getHeight());
}
//==============================================================================
