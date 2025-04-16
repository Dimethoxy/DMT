#pragma once

#include "utility/Settings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class Label : public juce::Component
{
  using Settings = dmt::Settings;
  using Justification = juce::Justification;
  const float& size = Settings::Window::size;
  const float& unixScale = Settings::unixFontScalingFactor;

public:
  Label(juce::String text,
        const juce::Font& font,
        const float& fontSize,
        const juce::Colour& colour,
        const Justification justification = Justification::centredTop,
        const bool multiline = false)
    : juce::Component()
    , text(text)
    , font(font)
    , fontSize(fontSize)
    , colour(colour)
    , justification(justification)
    , multiline(multiline)
  {
  }
  void paint(juce::Graphics& g)
  {
    TRACER("Label::paint");
    auto bounds = getLocalBounds();

    // Draw bounds debug
    g.setColour(juce::Colours::red);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1);

    // Set font size depending on OS
    if (OS_IS_WINDOWS) {
      g.setFont(font.withHeight(fontSize * size));
    } else {
      g.setFont(font.withHeight(fontSize * size * unixScale));
    }

    // Draw text
    if (!multiline) {
      g.setColour(colour);
      g.drawText(text, this->getLocalBounds(), justification, true);
    } else {
      const int startX = bounds.getX();
      const int baselineY = bounds.getY() + g.getCurrentFont().getAscent();
      const int maximumLineWidth = bounds.getWidth();
      g.setColour(colour);
      g.drawMultiLineText(
        text, startX, baselineY, maximumLineWidth, justification, 0.0f);
    }
  }

  inline void setText(const juce::String& newText) { this->text = newText; }
  inline juce::String getText() const { return this->text; }

private:
  bool multiline;
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