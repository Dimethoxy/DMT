//==============================================================================

#pragma once

#include "../../Utility/AppSettings.h"
#include "../../Utility/Shadow.h"
#include "../Widgets/TriangleButton.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
//==============================================================================
class Panel
  : public juce::Component
  , public juce::Button::Listener
{
  using Settings = dmt::AppSettings::Panel;
  using Fonts = dmt::AppSettings::Fonts;
  using Carousel = dmt::AppSettings::Carousel;
  // Global
  const float& size = dmt::AppSettings::size;
  const float& margin = dmt::AppSettings::Layout::margin;
  // General
  const juce::Colour& backgroundColour = Settings::backgroundColour;
  const float& cornerSize = Settings::cornerSize;
  // Border
  const bool& drawBorder = Settings::drawBorder;
  const juce::Colour& borderColour = Settings::borderColour;
  const float& borderStrength = Settings::borderStrength;
  // Shadows
  const bool& drawOuterShadow = Settings::drawOuterShadow;
  const bool& drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour& outerShadowColour = Settings::outerShadowColour;
  const juce::Colour& innerShadowColour = Settings::innerShadowColour;
  const float& outerShadowRadius = Settings::outerShadowRadius;
  const float& innerShadowRadius = Settings::innerShadowRadius;
  // Fonts
  const juce::Colour& fontColor = Settings::fontColor;
  const float& fontSize = Settings::fontSize;

public:
  Panel()
    : nextCallback([]() {})
    , prevCallback([]() {})
    , nextButton(dmt::gui::widgets::TriangleButton::right)
    , prevButton(dmt::gui::widgets::TriangleButton::left)
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
  {
    resized();
  }

  void paint(juce::Graphics& g) override
  {
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(margin);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = cornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // Draw outer shadow
    juce::Path outerShadowPath;
    if (drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    // Draw background if border is disabled
    if (!drawBorder) {
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(borderColour);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(innerBounds, innerCornerSize);
    }

    // Draw the inner shadow
    juce::Path innerShadowPath;
    if (drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }

    // Draw the title text
    auto textBounds = innerBounds;
    g.setColour(fontColor);
    auto titleFont = Fonts::regular.withHeight(fontSize * size);
    g.setFont(titleFont);
    g.drawText(
      getName(), textBounds.toNearestInt(), juce::Justification::centredTop, 1);
  }

  void resized() override
  {
    const auto bounds = getLocalBounds();
    auto leftBounds = bounds;
    auto rightBounds = bounds;

    leftBounds.removeFromRight(bounds.getWidth() -
                               Carousel::buttonWidth * size);
    leftBounds.setHeight(Carousel::buttonHeight * size);
    leftBounds.setCentre(leftBounds.getCentreX(), bounds.getCentreY());
    prevButton.setBounds(leftBounds.reduced(size * margin));

    rightBounds.removeFromLeft(bounds.getWidth() -
                               Carousel::buttonWidth * size);
    rightBounds.setHeight(Carousel::buttonHeight * size);
    rightBounds.setCentre(rightBounds.getCentreX(), bounds.getCentreY());
    nextButton.setBounds(rightBounds.reduced(size * margin));
  }

  virtual juce::String getName() { return "Panel"; }

  void setCallbacks(std::function<void()> next, std::function<void()> prev)
  {
    nextCallback = next;
    prevCallback = prev;
    addAndMakeVisible(nextButton);
    addAndMakeVisible(prevButton);
    nextButton.addListener(this);
    prevButton.addListener(this);
  }

  void next() { nextCallback(); }
  void prev() { prevCallback(); }
  void buttonClicked(juce::Button* button) override
  {
    if (button == &nextButton) {
      nextCallback();
    } else if (button == &prevButton) {
      prevCallback();
    }
  }

private:
  std::function<void()> nextCallback;
  std::function<void()> prevCallback;
  dmt::gui::widgets::TriangleButton nextButton;
  dmt::gui::widgets::TriangleButton prevButton;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};

//==============================================================================
} // namespace gui
} // namespace dmt
