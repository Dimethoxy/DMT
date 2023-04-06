//==============================================================================

#pragma once

#include "../Components/ArcButtonComponent.h"
#include "../Components/OscillatorDisplayComponent.h"
#include "../Components/TitleTopComponent.h"
#include "../Utility/AppSettings.h"
#include "../Widgets/LinearSliderComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class OscillatorPanel : public dmt::Panel
{
public:
  OscillatorPanel(juce::AudioProcessorValueTreeState& apvts)
    : prevButton(true)
    , nextButton(false)
    , pwmSlider(apvts, "Drive", "oscDrive", dmt::InfoUnit::Type::Symmetry)
    , biasSlider(apvts, "Bias", "oscBias", dmt::InfoUnit::Type::Symmetry)
  {
    addAndMakeVisible(top);
    addAndMakeVisible(oscDisplay);
    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(pwmSlider);
    addAndMakeVisible(biasSlider);
  }
  void update() override
  {
    top.setBounds(innerBounds.toNearestInt());
    top.setSize(top.getWidth(), top.getHeight() * 0.3f);
    top.setAlwaysOnTop(true);
    float dispalySize = 0.35f;
    oscDisplay.setSize(getWidth() * dispalySize, getWidth() * dispalySize);
    oscDisplay.setCentreRelative(0.5f, 0.45f);

    auto prevButtonX = oscDisplay.getX();
    auto prevButtonY = oscDisplay.getY() + oscDisplay.getHeight() / 2.0f;
    prevButton.setSize(oscDisplay.getWidth(), oscDisplay.getHeight());
    prevButton.setCentrePosition(prevButtonX, prevButtonY);

    auto nextButtonX = oscDisplay.getRight();
    auto nextButtonY = oscDisplay.getY() + oscDisplay.getHeight() / 2.0f;
    nextButton.setSize(oscDisplay.getWidth(), oscDisplay.getHeight());
    nextButton.setCentrePosition(nextButtonX, nextButtonY);

    pwmSlider.setBounds(prevButton.getX() - prevButton.getWidth() / 4.0f,
                        prevButton.getY(),
                        prevButton.getWidth() / 2.0f,
                        prevButton.getHeight());

    biasSlider.setBounds(nextButton.getRight() + nextButton.getWidth() / 4.0f -
                           pwmSlider.getWidth(),
                         nextButton.getY(),
                         nextButton.getWidth() / 2.0f,
                         nextButton.getHeight());
  }

private:
  dmt::TitleTopComponent top;
  dmt::OscillatorDisplayComponent oscDisplay;
  dmt::ArcButtonComponent prevButton;
  dmt::ArcButtonComponent nextButton;
  LinearSliderComponent pwmSlider;
  LinearSliderComponent biasSlider;
};
//==============================================================================
}
