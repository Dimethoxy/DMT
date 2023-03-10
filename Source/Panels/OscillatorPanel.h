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
class TitleTopComponent : public juce::Component
{
  using Settings = dmt::AppSettings::OscillatorTop;

public:
  TitleTopComponent()
  {
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
    fontShadow.radius = Settings::outerShadowRadius;
    fontShadow.colour = Settings::fontShadowColour;
  }
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    auto innerBounds = bounds.withHeight(bounds.getHeight() * 0.6f);
    auto cornerSize = dmt::AppSettings::Panel::borderSize;

    g.setColour(dmt::AppSettings::Colours::topground);
    g.fillRoundedRectangle(innerBounds, cornerSize);

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(innerBounds, cornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }
    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, cornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }

    juce::Font font =
      (AppSettings::Fonts::medium.withHeight(getHeight() * 0.5f));
    auto x = bounds.getX();
    auto y = bounds.getY() - bounds.getHeight() / 6.5f;
    auto w = bounds.getWidth();
    auto h = bounds.getHeight();
    juce::Path textPath;
    juce::GlyphArrangement glyphs;
    glyphs.addFittedText(
      font, "Classic Oscillator", x, y, w, h, juce::Justification::centred, 2);
    glyphs.createPath(textPath);

    if (Settings::drawFontShadow) {
      fontShadow.drawOuterForPath(g, textPath);
    }
    if (Settings::drawFontOutline) {
      g.setColour(Settings::fontOutlineColour);
      juce::PathStrokeType strokeType(Settings::fontOutlineThickness);
      g.strokePath(textPath, strokeType);
    }

    g.setColour(Settings::fontColour);
    g.fillPath(textPath);
  }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  dmt::Shadow fontShadow;
};
//==============================================================================

class AngleSlider : juce::Slider
{
  AngleSlider() { setSliderStyle(juce::Slider::LinearHorizontal); }
};

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
    float dispalySize = 0.5f;
    oscDisplay.setSize(getWidth() * dispalySize, getWidth() * dispalySize);
    oscDisplay.setCentreRelative(0.5f, 0.35f);

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
