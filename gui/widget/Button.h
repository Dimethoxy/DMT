#pragma once

#include "utility/Settings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class Button : public juce::Button
{
  using Settings = dmt::Settings;
  using Colour = juce::Colour;
  using Justification = juce::Justification;
  using ButtonSettings = dmt::Settings::Button;
  using String = juce::String;
  using Image = juce::Image;
  using ImageComponent = juce::ImageComponent;

  // General
  const float& size = Settings::Window::size;

  // Button
  const Colour& backgroundColour = ButtonSettings::backgroundColour;
  const Colour& outerShadowColour = ButtonSettings::outerShadowColour;
  const Colour& innerShadowColour = ButtonSettings::innerShadowColour;
  const float& rawCornerRadius = ButtonSettings::cornerRadius;
  const float& rawButtonPadding = ButtonSettings::padding;
  const float& outerShadowRadius = ButtonSettings::shadowRadius;
  const float& innerShadowRadius = ButtonSettings::shadowRadius;
  const bool& drawOuterShadow = ButtonSettings::drawOuterShadow;
  const bool& drawInnerShadow = ButtonSettings::drawInnerShadow;

public:
  Button(juce::String _name, juce::String _iconName)
    : juce::Button(_name)
    , rawSpecificSvgPadding(dmt::icons::getPadding(_iconName))
    , outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    icon = dmt::icons::getIcon(_iconName);
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
    addAndMakeVisible(backgroundImageComponent);
    addAndMakeVisible(iconImageComponent);
    addAndMakeVisible(hoverBackgroundImageComponent);
    hoverBackgroundImageComponent.setVisible(false);
  }

  ~Button() override {}

  void paintButton(juce::Graphics& g,
                   bool /*isMouseOverButton*/,
                   bool /*isButtonDown*/) override
  {
  }

  void mouseEnter(const juce::MouseEvent& event) override
  {
    backgroundImageComponent.setVisible(false);
    hoverBackgroundImageComponent.setVisible(true);
    repaint();
  }

  void mouseExit(const juce::MouseEvent& event) override
  {
    backgroundImageComponent.setVisible(true);
    hoverBackgroundImageComponent.setVisible(false);
    repaint();
  }

  void mouseDown(const juce::MouseEvent& event) override
  {
    backgroundImageComponent.setVisible(false);
    hoverBackgroundImageComponent.setVisible(true);
    repaint();
  }

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto buttonPadding = rawButtonPadding * size;
    auto innerBounds = bounds.reduced(buttonPadding);
    const auto cornerRadius = rawCornerRadius * size;

    // Set the bounds for the background
    backgroundImage =
      Image(Image::ARGB, innerBounds.getWidth(), innerBounds.getHeight(), true);
    backgroundImageComponent.setBounds(innerBounds);
    hoverBackgroundImage =
      Image(Image::ARGB, innerBounds.getWidth(), innerBounds.getHeight(), true);
    hoverBackgroundImageComponent.setBounds(innerBounds);

    // Set the bounds for the outer shadow
    juce::Path outerShadowPath;
    outerShadowPath.addRoundedRectangle(innerBounds, cornerRadius);
    outerShadow.setPath(outerShadowPath);
    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

    // Set the bounds for the inner shadow
    juce::Path innerShadowPath;
    innerShadowPath.addRoundedRectangle(innerBounds, cornerRadius);
    innerShadow.setPath(innerShadowPath);
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

    // Reorder the components
    innerShadow.toBack();
    outerShadow.toBack();
    backgroundImageComponent.toBack();

    // Set the bounds for the iconImageComponent and draw the icon
    const auto specificSvgPadding = rawSpecificSvgPadding * size;
    const auto globalSvgPadding = 2.5f * size;
    const auto svgPadding = specificSvgPadding + globalSvgPadding;
    const auto iconArea = innerBounds.reduced(svgPadding);
    iconImage = juce::Image(
      juce::Image::ARGB, iconArea.getWidth(), iconArea.getHeight(), true);

    iconImageComponent.setBounds(iconArea);
    iconImageComponent.setAlwaysOnTop(true);

    // Paint all the components
    drawBackground();
    drawIcon();
  }

private:
  void drawIcon()
  {
    if (icon != nullptr) {
      juce::Graphics iconGraphics(iconImage);
      iconGraphics.fillAll(
        juce::Colours::transparentBlack); // Clear previous icon
      const auto iconArea = iconImage.getBounds().toFloat();
      icon->replaceColour(juce::Colours::black, juce::Colours::white);
      icon->drawWithin(
        iconGraphics, iconArea, juce::RectanglePlacement::centred, 1.0f);
      iconImageComponent.setImage(iconImage);
    }
  }

  void drawBackground()
  {
    // Get the bounds for the background
    const auto cornerRadius = rawCornerRadius * size;
    const auto backgroundArea = backgroundImage.getBounds().toFloat();

    // Normal background
    juce::Graphics backgroundGraphics(backgroundImage);
    backgroundGraphics.fillAll(juce::Colours::transparentBlack);
    backgroundGraphics.setColour(backgroundColour);
    backgroundGraphics.fillRoundedRectangle(backgroundArea, cornerRadius);
    backgroundImageComponent.setImage(backgroundImage);

    // Hover background
    juce::Graphics hoverGraphics(hoverBackgroundImage);
    hoverGraphics.fillAll(juce::Colours::transparentBlack);
    hoverGraphics.setColour(juce::Colours::red);
    hoverGraphics.fillRoundedRectangle(backgroundArea, cornerRadius);
    hoverBackgroundImageComponent.setImage(hoverBackgroundImage);
  }

  const float rawSpecificSvgPadding;
  std::unique_ptr<juce::Drawable> icon;
  Shadow outerShadow;
  Shadow innerShadow;

  Image backgroundImage;
  ImageComponent backgroundImageComponent;
  Image hoverBackgroundImage;
  ImageComponent hoverBackgroundImageComponent;
  Image iconImage;
  ImageComponent iconImageComponent;
};
} // namespace widget
} // namespace dmt
} // namespace gui