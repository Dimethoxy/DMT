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
    iconImage = Image(Image::ARGB, getWidth(), getHeight(), true);
    iconImage.setPixelAt(2, 2, juce::Colours::red);
    iconImageComponent.setImage(iconImage);

    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
    addAndMakeVisible(iconImageComponent);
  }

  ~Button() override {}

  void paintButton(juce::Graphics& g,
                   bool /*isMouseOverButton*/,
                   bool /*isButtonDown*/) override
  {
    auto bounds = getLocalBounds();
    const auto buttonPadding = rawButtonPadding * size;
    auto innerBounds = bounds.reduced(buttonPadding);

    const auto cornerRadius = rawCornerRadius * size;
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(innerBounds.toFloat(), cornerRadius);

    if (icon != nullptr) {
      iconImage = Image(Image::ARGB, getWidth(), getHeight(), true);
      juce::Graphics iconGraphics(iconImage);
      iconGraphics.fillAll(
        juce::Colours::transparentBlack); // Clear previous icon
      const auto specificSvgPadding = rawSpecificSvgPadding * size;
      const auto globalSvgPadding = 2.5f * size;
      const auto svgPadding = specificSvgPadding + globalSvgPadding;
      const auto iconArea = innerBounds.reduced(svgPadding).toFloat();
      icon->replaceColour(juce::Colours::black, juce::Colours::white);
      icon->drawWithin(
        iconGraphics, iconArea, juce::RectanglePlacement::centred, 1.0f);
      iconImageComponent.setImage(iconImage);
    }
  }

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto buttonPadding = rawButtonPadding * size;
    auto innerBounds = bounds.reduced(buttonPadding);
    const auto cornerRadius = rawCornerRadius * size;

    juce::Path outerShadowPath;
    outerShadowPath.addRoundedRectangle(innerBounds, cornerRadius);
    outerShadow.setPath(outerShadowPath);
    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    outerShadow.toBack();

    juce::Path innerShadowPath;
    innerShadowPath.addRoundedRectangle(innerBounds, cornerRadius);
    innerShadow.setPath(innerShadowPath);
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    innerShadow.toBack();

    // Set the bounds for the iconImage
    iconImageComponent.setBounds(bounds);
    iconImageComponent.setAlwaysOnTop(true);
  }

private:
  std::unique_ptr<juce::Drawable> icon;
  const float rawSpecificSvgPadding;
  ImageComponent iconImageComponent;
  Image iconImage;
  juce::Rectangle<int> iconImageArea;
  Shadow outerShadow;
  Shadow innerShadow;
};
} // namespace widget
} // namespace dmt
} // namespace gui