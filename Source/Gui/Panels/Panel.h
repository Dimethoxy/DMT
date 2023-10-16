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
  using Settings = dmt::AppSettings;
  using Colours = Settings::Colours;
  using Fonts = Settings::Fonts;
  using Carousel = Settings::Carousel;
  const float& size = Settings::size;
  const float& margin = Settings::Layout::margin;
  const bool& drawOuterShadow = Settings::Appearance::drawOuterShadow;
  const bool& drawInnerShadow = Settings::Appearance::drawInnerShadow;
  const bool& drawBorder = Settings::Appearance::drawBorder;
  const float& borderStrength = Settings::Appearance::borderStrength;
  const float& cornerSize = Settings::Appearance::cornerSize;

public:
  Panel()
    : nextCallback([]() {})
    , prevCallback([]() {})
    , nextButton(dmt::gui::widgets::TriangleButton::right)
    , prevButton(dmt::gui::widgets::TriangleButton::left)
  {
    outerShadow.radius = Settings::Appearance::shadowRadius;
    outerShadow.colour = Settings::Colours::outerShadow;
    innerShadow.radius = Settings::Appearance::shadowRadius;
    innerShadow.colour = Settings::Colours::innerShadow;
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
      g.setColour(Colours::solidDark);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(Colours::solidMid);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
      g.setColour(Colours::solidDark);
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
    g.setColour(Colours::foreground);
    auto titleFont = Fonts::regular.withHeight(Fonts::panelTitleSize * size);
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
