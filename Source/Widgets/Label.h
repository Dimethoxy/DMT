#pragma once

#include "../Utility/AppSettings.h"
#include <JuceHeader.h>

namespace dmt {
class Label : public juce::Label
{
public:
  Label(juce::String name, float fontSize, juce::Font font)
    : fontSize(fontSize)
  {
    setText(name, juce::NotificationType::dontSendNotification);
    setJustificationType(juce::Justification::centredTop);
    setColour(juce::Label::ColourIds::textColourId,
              AppSettings::Fonts::labelFontColour);
    setFont(font);
  }
  void resized()
  {
    setFont(getFont().withHeight(fontSize * AppSettings::size));
  }

private:
  float fontSize;
  bool isTitle = false;
};
}