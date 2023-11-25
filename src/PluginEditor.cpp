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
  , sendPanelA(p.apvts, juce::String("A"))
  , sendPanelB(p.apvts, juce::String("B"))
  , sendPanelC(p.apvts, juce::String("C"))
  , keyboardComponent(p.keyboardState,
                      juce::MidiKeyboardComponent::horizontalKeyboard)
{
  using Settings = dmt::AppSettings;
  using Layout = dmt::AppSettings::Layout;
  const float& leftWidth = Layout::leftWidth;
  const float& centerWidth = Layout::centerWidth;
  const float& rightWidth = Layout::rightWidth;

  addAndMakeVisible(oscillatorPanel);
  addAndMakeVisible(voicingPanel);

  addAndMakeVisible(gainPanel);
  addAndMakeVisible(pitchPanel);
  addAndMakeVisible(distortionPanel);

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
  const float rawTabHeight = Settings::Layout::tabHeight + 2.0f * margin;
  const float tabHeight = rawTabHeight * size;
  const float rawRowHeight = Settings::Layout::rowHeight + 2.0f * margin;
  const float rowHeight = rawRowHeight * size;
  const float rawKeyboardHeight = Settings::Layout::keyboardHeight;
  const float keyboardHeight = rawKeyboardHeight * size;

  auto innerBounds = bounds.reduced(margin);
  const auto headerBounds = innerBounds.removeFromTop(headerHeight);
  const auto tabBounds = innerBounds.removeFromTop(tabHeight);
  const auto topBounds = innerBounds.removeFromTop(rowHeight);
  const auto midBounds = innerBounds.removeFromTop(rowHeight);
  const auto bottomBounds = innerBounds.removeFromTop(rowHeight);
  const auto keyboardBounds =
    juce::Rectangle<int>(0, height - keyboardHeight, width, keyboardHeight);

  innerBounds = bounds.reduced(margin);
  const auto leftBounds = innerBounds.removeFromLeft(leftWidth);
  const auto centerBounds = innerBounds.removeFromLeft(centerWidth);
  const auto rightBounds = innerBounds.removeFromLeft(rightWidth);

  oscillatorPanel.setBounds(leftBounds.getX(),
                            topBounds.getY(),
                            leftBounds.getWidth(),
                            topBounds.getHeight() + bottomBounds.getHeight());
  voicingPanel.setBounds(leftBounds.getX(),
                         bottomBounds.getY(),
                         leftBounds.getWidth(),
                         bottomBounds.getHeight());
  gainPanel.setBounds(centerBounds.getX(),
                      topBounds.getY(),
                      centerBounds.getWidth(),
                      topBounds.getHeight());
  pitchPanel.setBounds(centerBounds.getX(),
                       midBounds.getY(),
                       centerBounds.getWidth(),
                       midBounds.getHeight());
  distortionPanel.setBounds(centerBounds.getX(),
                            bottomBounds.getY(),
                            centerBounds.getWidth(),
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
  keyboardComponent.setBounds(keyboardBounds);
}
//==============================================================================
