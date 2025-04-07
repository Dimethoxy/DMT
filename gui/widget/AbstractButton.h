#pragma once
//==============================================================================
#include "gui/widget/Shadow.h"
#include "utility/Icon.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class AbstractHeaderButton : public juce::Button
{
  using Settings = dmt::Settings;
  using Colour = juce::Colour;
  using Image = juce::Image;
  using ImageComponent = juce::ImageComponent;
  using ButtonSettings = dmt::Settings::Button;

  // General
  const float& size = Settings::Window::size;

  // Button
  const Colour& backgroundColour = ButtonSettings::backgroundColour;
  const Colour& outerShadowColour = ButtonSettings::outerShadowColour;
  const Colour& innerShadowColour = ButtonSettings::innerShadowColour;
  const Colour& fontColour = ButtonSettings::fontColour;
  const Colour& hoverColour = ButtonSettings::hoverColour;
  const Colour& clickColour = ButtonSettings::clickColour;
  const float& rawCornerRadius = ButtonSettings::cornerRadius;
  const float& rawButtonPadding = ButtonSettings::padding;
  const float& outerShadowRadius = ButtonSettings::outerShadowRadius;
  const float& innerShadowRadius = ButtonSettings::innerShadowRadius;
  const bool& drawOuterShadow = ButtonSettings::drawOuterShadow;
  const bool& drawInnerShadow = ButtonSettings::drawInnerShadow;

public:
  AbstractHeaderButton(juce::String _name, juce::String _iconName)
    : juce::Button(_name)
    , rawSpecificSvgPadding(dmt::icons::getPadding(_iconName))
  {
    icon = dmt::icons::getIcon(_iconName);
    addAndMakeVisible(backgroundImageComponent);
    addAndMakeVisible(hoverBackgroundImageComponent);
    addAndMakeVisible(clickedBackgroundImageComponent);
    addAndMakeVisible(iconImageComponent);
    addAndMakeVisible(hoverIconImageComponent);

    hoverBackgroundImageComponent.setVisible(false);
    clickedBackgroundImageComponent.setVisible(false);
    hoverIconImageComponent.setVisible(false);
    addMouseListener(this, true);
  }

  ~AbstractHeaderButton() override = default;

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto buttonPadding = rawButtonPadding * size;
    auto innerBounds = bounds.reduced(buttonPadding);

    setBackgroundBounds(innerBounds);
    setIconBounds(innerBounds);

    drawBackground();
    drawIcon();
  }

  void setPassiveState()
  {
    backgroundImageComponent.setVisible(true);
    hoverBackgroundImageComponent.setVisible(false);
    clickedBackgroundImageComponent.setVisible(false);
    iconImageComponent.setVisible(true);
    hoverIconImageComponent.setVisible(false);
  }

  void setHoverState()
  {
    backgroundImageComponent.setVisible(false);
    hoverBackgroundImageComponent.setVisible(true);
    clickedBackgroundImageComponent.setVisible(false);
    iconImageComponent.setVisible(false);
    hoverIconImageComponent.setVisible(true);
  }

  void setClickedState()
  {
    backgroundImageComponent.setVisible(false);
    hoverBackgroundImageComponent.setVisible(false);
    clickedBackgroundImageComponent.setVisible(true);
    iconImageComponent.setVisible(false);
    hoverIconImageComponent.setVisible(true);
  }

  void paintButton(juce::Graphics& /*g*/,
                   bool /*isMouseOverButton*/,
                   bool /*isButtonDown*/) override
  {
  }

private:
  void setBackgroundBounds(const juce::Rectangle<int>& innerBounds)
  {
    backgroundImage =
      Image(Image::ARGB, innerBounds.getWidth(), innerBounds.getHeight(), true);
    backgroundImageComponent.setBounds(innerBounds);
    hoverBackgroundImage =
      Image(Image::ARGB, innerBounds.getWidth(), innerBounds.getHeight(), true);
    hoverBackgroundImageComponent.setBounds(innerBounds);
    clickedBackgroundImage =
      Image(Image::ARGB, innerBounds.getWidth(), innerBounds.getHeight(), true);
    clickedBackgroundImageComponent.setBounds(innerBounds);
  }

  void setIconBounds(const juce::Rectangle<int>& innerBounds)
  {
    const auto specificSvgPadding = rawSpecificSvgPadding * size;
    const auto globalSvgPadding = 2.5f * size;
    const auto svgPadding = specificSvgPadding + globalSvgPadding;

    const auto iconArea = innerBounds.reduced(svgPadding);
    iconImage = juce::Image(
      juce::Image::ARGB, iconArea.getWidth(), iconArea.getHeight(), true);
    iconImageComponent.setBounds(iconArea);

    hoverIconImage = juce::Image(
      juce::Image::ARGB, iconArea.getWidth(), iconArea.getHeight(), true);
    hoverIconImageComponent.setBounds(iconArea);
  }

  void drawBackground()
  {
    const auto cornerRadius = rawCornerRadius * size;
    const auto backgroundArea = backgroundImage.getBounds().toFloat();

    juce::Graphics backgroundGraphics(backgroundImage);
    backgroundGraphics.fillAll(juce::Colours::transparentBlack);
    backgroundGraphics.setColour(backgroundColour);
    backgroundGraphics.fillRoundedRectangle(backgroundArea, cornerRadius);
    backgroundImageComponent.setImage(backgroundImage);

    juce::Graphics hoverGraphics(hoverBackgroundImage);
    hoverGraphics.fillAll(juce::Colours::transparentBlack);
    hoverGraphics.setColour(hoverColour);
    hoverGraphics.fillRoundedRectangle(backgroundArea, cornerRadius);
    hoverBackgroundImageComponent.setImage(hoverBackgroundImage);

    juce::Graphics clickGraphics(clickedBackgroundImage);
    clickGraphics.fillAll(juce::Colours::transparentBlack);
    clickGraphics.setColour(clickColour);
    clickGraphics.fillRoundedRectangle(backgroundArea, cornerRadius);
    clickedBackgroundImageComponent.setImage(clickedBackgroundImage);
  }

  void drawIcon()
  {
    if (icon != nullptr) {
      juce::Graphics iconGraphics(iconImage);
      iconGraphics.fillAll(juce::Colours::transparentBlack);
      const auto iconArea = iconImage.getBounds().toFloat();
      const auto clonedIcon = icon->createCopy();
      clonedIcon->replaceColour(juce::Colours::black, juce::Colours::white);
      clonedIcon->drawWithin(
        iconGraphics, iconArea, juce::RectanglePlacement::centred, 1.0f);
      iconImageComponent.setImage(iconImage);

      juce::Graphics hoverIconGraphics(hoverIconImage);
      hoverIconGraphics.fillAll(juce::Colours::transparentBlack);
      hoverIconGraphics.setColour(hoverColour);
      const auto clonedHoverIcon = icon->createCopy();
      clonedHoverIcon->replaceColour(juce::Colours::black,
                                     juce::Colours::black);
      clonedHoverIcon->drawWithin(
        hoverIconGraphics, iconArea, juce::RectanglePlacement::centred, 1.0f);
      hoverIconImageComponent.setImage(hoverIconImage);
    }
  }

  const float rawSpecificSvgPadding;
  std::unique_ptr<juce::Drawable> icon;

  Image backgroundImage;
  ImageComponent backgroundImageComponent;
  Image hoverBackgroundImage;
  ImageComponent hoverBackgroundImageComponent;
  Image clickedBackgroundImage;
  ImageComponent clickedBackgroundImageComponent;
  Image iconImage;
  ImageComponent iconImageComponent;
  Image hoverIconImage;
  ImageComponent hoverIconImageComponent;
};
} // namespace widget
} // namespace dmt
} // namespace gui