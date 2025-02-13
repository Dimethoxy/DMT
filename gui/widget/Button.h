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
  Button(juce::String _name)
    : juce::Button(_name)
  {
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
  }

private:
  std::function<void()> onClick;
};
} // namespace widget
} // namespace dmt
} // namespace gui