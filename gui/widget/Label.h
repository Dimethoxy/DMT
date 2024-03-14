#pragma once

#include "../../utility/LibrarySettings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class Label : public juce::Component
{
  using LibrarySettings = dmt::LibrarySettings;
  using Justification = juce::Justification;
  const float& size = LibrarySettings::Layout::size;
  const float& macFontScale = LibrarySettings::macFontScale;

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
    g.setColour(juce::Colours::red);
    if (LibrarySettings::debugBounds)
      g.drawRect(bounds, 1);

    // Set font size depending on OS
    if (OS_IS_DARWIN) {
      g.setFont(font.withHeight(fontSize * size * macFontScale));
    } else {
      g.setFont(font.withHeight(fontSize * size));
    }

    // Draw text
    g.setColour(colour);
    g.drawText(text, this->getLocalBounds(), justification, true);
  }

  inline void setText(const juce::String& newText) { this->text = newText; }

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