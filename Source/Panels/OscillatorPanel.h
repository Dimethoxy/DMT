//==============================================================================

#pragma once

#include "../Components/ArcButtonComponent.h"
#include "../Components/OscillatorDisplayComponent.h"
#include "../Utility/AppSettings.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================

//==============================================================================
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel()
    : prevButton(true)
    , nextButton(false)
  {
    addAndMakeVisible(oscDisplay);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
  }
  void resized() override
  {
    float dispalySize = 0.5f;
    oscDisplay.setSize(getWidth() * dispalySize, getWidth() * dispalySize);
    oscDisplay.setCentreRelative(0.5f, 0.4f);

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
  dmt::OscillatorDisplayComponent oscDisplay;
  dmt::ArcButtonComponent prevButton;
  dmt::ArcButtonComponent nextButton;
};
//==============================================================================
}
