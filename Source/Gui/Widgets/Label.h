#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class Label : juce::Component
{
public:
  Label(juce::String text,
        juce::Font& font,
        float& fontSize,
        juce::Colour& colour)
    : text(text)
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

private:
  juce::String text;
  juce::Font& font;
  float& fontSize;
  juce::Colour& colour;
};
} // namespace widgets
} // namespace gui
} // namespace dmt