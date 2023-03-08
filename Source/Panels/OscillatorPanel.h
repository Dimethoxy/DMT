//==============================================================================

#pragma once

#include "../Components/OscillatorDisplayComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
class ArcButton : public juce::Component
{
public:
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    g.setColour(juce::Colours::white);
    g.drawRect(bounds, 1.0f);
  }

  juce::Path getPath(juce::Rectangle<int> bounds)
  {
    juce::Path path;

    int midX = bounds.getX();
    int midY = bounds.getCentreY();

    int topX = bounds.getX() + bounds.getHeight();
    int topY = bounds.getY();

    return path;
  }

private:
};
//==============================================================================
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel()
  {
    addAndMakeVisible(oscDisplay);
    addAndMakeVisible(prevButton);
  }
  void resized() override
  {
    oscDisplay.setBoundsRelative(0.2f, 0.15f, 0.6f, 0.3f);
    oscDisplay.setSize(oscDisplay.getHeight(), oscDisplay.getHeight());

    auto prevButtonX = oscDisplay.getX();
    auto prevButtonY = oscDisplay.getY() + oscDisplay.getHeight() / 2.0f;
    prevButton.setSize(oscDisplay.getWidth() / 1.5f, oscDisplay.getHeight());
    prevButton.setCentrePosition(prevButtonX, prevButtonY);
  }

private:
  dmt::OscillatorDisplayComponent oscDisplay;
  dmt::ArcButton prevButton;
};
//==============================================================================
}
