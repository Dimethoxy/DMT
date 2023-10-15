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
  addAndMakeVisible(voicingPanel);

  addAndMakeVisible(analogGainPanel);
  addAndMakeVisible(modernGainPanel);
  addAndMakeVisible(modernGainPanel);

  // gainCarousel.init({ dynamic_cast<dmt::gui::Panel*>(&analogGainPanel),
  //                     dynamic_cast<dmt::gui::Panel*>(&modernGainPanel) });

  addAndMakeVisible(sendPanelA);
  addAndMakeVisible(sendPanelB);
  addAndMakeVisible(sendPanelC);

  addAndMakeVisible(keyboardComponent);
  setSize(1805, 1160);
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
