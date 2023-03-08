//==============================================================================

#pragma once

#include "../Components/OscillatorDisplayComponent.h"
#include "../Utility/AppSettings.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
class ArcButtonHitbox
  : public juce::Component
  , public juce::MouseListener
{
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    g.setColour(juce::Colours::white);
    g.fillRect(bounds);
  }
};
class ArcButton : public juce::Component
{
public:
  using Settings = dmt::AppSettings::ArcButton;
  ArcButton(bool leftSided)
  {
    this->leftSided = leftSided;
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    juce::Path path = getPath();

    if (isMouseOver()) {
      g.setColour(Settings::hoverColour);
    } else {
      g.setColour(Settings::foregroundColour);
    }

    g.fillPath(path);

    if (Settings::drawInnerShadow)
      innerShadow.drawInnerForPath(g, path);
    if (Settings::drawOuterShadow)
      outerShadow.drawOuterForPath(g, path);

    g.setColour(Settings::borderColour);
    g.strokePath(path, juce::PathStrokeType(Settings::borderSize));
  }
  void resized()
  {
    auto bounds = this->getLocalBounds().toFloat();
    bounds.reduce(bounds.getWidth() / 3.75f, bounds.getHeight() / 5.5f);
    if (!hitbox.isMouseButtonDown())
      bounds = bounds.reduced(10.0f);

    auto offsetDirection = leftSided ? -1.0f : 1.0f;
    auto xOffset = bounds.getWidth() / 8.0f * offsetDirection;
    bounds.setX(bounds.getX() + xOffset);
    hitbox.setBounds(bounds.toNearestInt());
  }
  //============================================================================
  juce::Path getPath()
  {
    const auto bounds = hitbox.getBounds().toFloat();
    juce::Path path;

    float midX = leftSided ? bounds.getX() : bounds.getRight();
    float midY = bounds.getCentreY();
    juce::Point<float> mid(midX, midY);
    path.startNewSubPath(mid);

    int topX = leftSided ? bounds.getRight() : bounds.getX();
    int topY = bounds.getY();
    juce::Point<float> top(topX, topY);
    path.lineTo(top);

    float downX = leftSided ? bounds.getRight() : bounds.getX();
    float downY = bounds.getY() + bounds.getHeight();
    juce::Point<float> down(downX, downY);
    path.quadraticTo(bounds.getCentre(), down);

    path.closeSubPath();
    return path;
  }
  //============================================================================
private:
  bool leftSided;
  dmt::ArcButtonHitbox hitbox;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
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
    oscDisplay.setBoundsRelative(0.2f, 0.15f, 0.6f, 0.3f);
    oscDisplay.setSize(oscDisplay.getHeight(), oscDisplay.getHeight());
    oscDisplay.setCentreRelative(0.5f, 0.2f);

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
  dmt::ArcButton prevButton;
  dmt::ArcButton nextButton;
};
//==============================================================================
}
