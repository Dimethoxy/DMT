#pragma once

#include "dmt/gui/widget/Button.h"
#include "dmt/gui/widget/Label.h"
#include "dmt/utility/Fonts.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {
class Header : public juce::Component
{
  // Aliases
  using Button = dmt::gui::widget::Button;
  using Label = dmt::gui::widget::Label;
  using String = juce::String;
  using Colour = juce::Colour;
  using Fonts = dmt::utility::Fonts;
  using HeaderSettings = dmt::Settings::Header;

  // Window
  const float& size = dmt::Settings::Window::size;

  // Panel
  const float& rawBorderStrength = dmt::Settings::Panel::borderStrength;

  // Header
  const String name = ProjectInfo::projectName;
  const Colour& backroundColour = HeaderSettings::backroundColour;
  const Colour& borderColor = HeaderSettings::borderColor;
  const Colour& titleFontColour = HeaderSettings::titleFontColour;
  const Colour& buttonColour = HeaderSettings::buttonColour;
  const float& titleFontSize = HeaderSettings::titleFontSize;
  const float& rawTitleOffset = HeaderSettings::titleOffset;
  const float& rawTitleButtonWidth = HeaderSettings::titleButtonWidth;

  // Button
  const float& rawButtonPadding = dmt::Settings::Button::padding;

public:
  Header(juce::String titleText)
    : title(String("ProjectLabel"),
            fonts.display,
            titleFontSize,
            Colours::white,
            juce::Justification::centred)
    , settingsButton("SettingsButton", "Settings")
  {
    title.setText(titleText);
    addAndMakeVisible(title);
    addAndMakeVisible(settingsButton);
  };

  ~Header() override {}

  void paint(juce::Graphics& g) override
  {
    // Paint the border
    auto bounds = getLocalBounds();
    g.setColour(borderColor);
    g.fillRect(bounds);

    // Paint the background
    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);
    g.setColour(backroundColour);
    g.fillRect(bounds);

    // Paint rounded rectangle behind the title
    const auto titleButtonWidth = rawTitleButtonWidth * size;
    const auto buttonPadding = rawButtonPadding * size;
    const auto titleBounds = bounds.withWidth(titleButtonWidth)
                               .withCentre(bounds.getCentre())
                               .reduced(buttonPadding);
    g.setColour(buttonColour);
    g.fillRoundedRectangle(titleBounds.toFloat(), 20.0f);
  }

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto titleOffset = rawTitleOffset * size;
    const auto titleBounds = bounds.withY(titleOffset);
    title.setBounds(titleBounds);

    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);
    const auto settingsButtonBounds = bounds.removeFromRight(48.0f * size);
    settingsButton.setBounds(settingsButtonBounds);
  }

private:
  Label title;
  Fonts fonts;
  Button settingsButton;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt