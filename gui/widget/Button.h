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
    auto innerBounds = bounds.reduced(10 * size);
    g.setColour(Colour(40, 42, 54));
    g.fillRoundedRectangle(innerBounds.toFloat(), 10.0f * size);
  }

private:
  std::function<void()> onClick;
};
} // namespace widget
} // namespace dmt
} // namespace gui