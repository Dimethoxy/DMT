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

  // General
  const float& size = Settings::Window::size;

  // Button
  const Colour& backgroundColour = ButtonSettings::backgroundColour;
  const float& rawCornerRadius = ButtonSettings::cornerRadius;
  const float& rawButtonPadding = ButtonSettings::padding;

public:
  Button(juce::String _name, juce::String _iconName)
    : juce::Button(_name)
    , rawSpecificSvgPadding(dmt::icons::getPadding(_iconName))
  {
    icon = dmt::icons::getIcon(_iconName);
  }

  ~Button() override {}

  void paintButton(juce::Graphics& g,
                   bool isMouseOverButton,
                   bool isButtonDown) override
  {
    auto bounds = getLocalBounds();
    const auto buttonPadding = rawButtonPadding * size;
    auto innerBounds = bounds.reduced(buttonPadding);

    const auto cornerRadius = rawCornerRadius * size;
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(innerBounds.toFloat(), cornerRadius * size);

    const auto specificSvgPadding = rawSpecificSvgPadding * size;
    const auto globalSvgPadding = 2.5f * size;
    const auto svgPadding = specificSvgPadding + globalSvgPadding;
    const auto iconArea = innerBounds.reduced(svgPadding).toFloat();
    g.setColour(juce::Colours::white);
    icon->replaceColour(juce::Colours::black, juce::Colours::white);
    icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred, 1.0f);
  }

private:
  std::function<void()> onClick;
  std::unique_ptr<juce::Drawable> icon;
  const float rawSpecificSvgPadding;
};
} // namespace widget
} // namespace dmt
} // namespace gui