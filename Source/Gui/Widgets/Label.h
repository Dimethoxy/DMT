#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class Label : public juce::Component
{
  using Settings = dmt::AppSettings;
  using Justification = juce::Justification;
  const float& size = Settings::Layout::size;

public:
  Label(juce::String text,
        const juce::Font& font,
        const float& fontSize,
        const juce::Colour& colour,
        const Justification justification = Justification::centredTop)
    : juce::Component()
    , text(text)
    , font(font)
    , fontSize(fontSize)
    , colour(colour)
    , justification(justification)
  {
  }
  void paint(juce::Graphics& g)
  {
    auto bounds = getLocalBounds();

    // Draw bounds debug
    // g.setColour(juce::Colours::aliceblue);
    // g.drawRect(bounds, 1.0f);

    g.setFont(fontSize * size);
    g.setColour(colour);
    g.drawText(text, this->getLocalBounds(), justification, true);
  }

  void setText(juce::String text) noexcept { this->text = text; }

private:
  juce::String text;
  const juce::Font& font;
  const float& fontSize;
  const juce::Colour& colour;
  Justification justification;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
};
} // namespace widgets
} // namespace gui
} // namespace dmt