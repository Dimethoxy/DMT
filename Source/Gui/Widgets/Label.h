#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class Label : public juce::Component
{
public:
  Label(juce::String text,
        const juce::Font& font,
        const float& fontSize,
        const juce::Colour& colour)
    : juce::Component()
    , text(text)
    , font(font)
    , fontSize(fontSize)
    , colour(colour)
  {
  }
  void paint(juce::Graphics& g)
  {
    g.setFont(fontSize);
    g.setColour(colour);
    g.drawText(
      text, this->getLocalBounds(), juce::Justification::centred, true);
  }

  void setText(juce::String text) noexcept { this->text = text; }

private:
  juce::String text;
  const juce::Font& font;
  const float& fontSize;
  const juce::Colour& colour;
};
} // namespace widgets
} // namespace gui
} // namespace dmt