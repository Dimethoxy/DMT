//==============================================================================

#pragma once

#include "../Components/ArcButtonComponent.h"
#include "../Components/OscillatorDisplayComponent.h"
#include "../Components/TitleTopComponent.h"
#include "../Utility/AppSettings.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel()
    : prevButton(true)
    , nextButton(false)
  {
    addAndMakeVisible(top);
    addAndMakeVisible(oscDisplay);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
  }
  void update() override
  {
    top.setBounds(innerBounds.toNearestInt());
    top.setSize(top.getWidth(), top.getHeight() * 0.15f);
    float dispalySize = 0.40f;
    oscDisplay.setSize(getWidth() * dispalySize, getWidth() * dispalySize);
    oscDisplay.setCentreRelative(0.5f, 0.30f);

    auto prevButtonX = oscDisplay.getX();
    auto prevButtonY = oscDisplay.getY() + oscDisplay.getHeight() / 2.0f;
    prevButton.setSize(oscDisplay.getWidth(), oscDisplay.getHeight());
    prevButton.setCentrePosition(prevButtonX, prevButtonY);

    auto nextButtonX = oscDisplay.getRight();
    auto nextButtonY = oscDisplay.getY() + oscDisplay.getHeight() / 2.0f;
    nextButton.setSize(oscDisplay.getWidth(), oscDisplay.getHeight());
    nextButton.setCentrePosition(nextButtonX, nextButtonY);
  }

private:
  dmt::TitleTopComponent top;
  dmt::OscillatorDisplayComponent oscDisplay;
  dmt::ArcButtonComponent prevButton;
  dmt::ArcButtonComponent nextButton;
};
//==============================================================================
}
