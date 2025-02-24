#pragma once
//==============================================================================
#include "dmt/gui/widget/Button.h"
#include "dmt/gui/widget/Label.h"
#include "dmt/gui/widget/Shadow.h"
#include "dmt/utility/Fonts.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace window {
//==============================================================================
class Header : public juce::Component
{
  // Aliases
  using Button = dmt::gui::widget::Button;
  using Label = dmt::gui::widget::Label;
  using String = juce::String;
  using Colour = juce::Colour;
  using Fonts = dmt::utility::Fonts;
  using HeaderSettings = dmt::Settings::Header;
  using PanelSettings = dmt::Settings::Panel;
  using Shadow = dmt::gui::widget::Shadow;

  // Window
  const float& size = dmt::Settings::Window::size;

  // Panel
  const Colour& outerShadowColour = PanelSettings::outerShadowColour;
  const Colour& innerShadowColour = PanelSettings::innerShadowColour;
  const float& rawBorderStrength = PanelSettings::borderStrength;
  const float& outerShadowRadius = PanelSettings::outerShadowRadius;
  const float& innerShadowRadius = PanelSettings::innerShadowRadius;
  const bool& drawOuterShadow = PanelSettings::drawOuterShadow;
  const bool& drawInnerShadow = PanelSettings::drawInnerShadow;

  // Header
  const String name = ProjectInfo::projectName;
  const Colour& backroundColour = HeaderSettings::backroundColour;
  const Colour& borderColor = HeaderSettings::borderColor;
  const Colour& titleFontColour = HeaderSettings::titleFontColour;
  const Colour& buttonColour = HeaderSettings::buttonColour;

  const float& titleFontSize = HeaderSettings::titleFontSize;
  const float& rawTitleOffset = HeaderSettings::titleOffset;
  const float& rawTitleButtonWidth = HeaderSettings::titleButtonWidth;
  const float& rawHeaderButtonWidth = HeaderSettings::buttonWidth;

  // Button
  const float& rawButtonPadding = dmt::Settings::Button::padding;

public:
  Header(juce::String titleText)
    : outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
    , title(String("ProjectLabel"),
            fonts.display,
            titleFontSize,
            Colours::white,
            juce::Justification::centred)
    , settingsButton("HeaderSettingsButton", "Settings")
    , titleButton("HeaderTitleButton", "None")
  {
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
    addAndMakeVisible(title);
    addAndMakeVisible(settingsButton);
    addAndMakeVisible(titleButton);
    title.setText(titleText);
    titleButton.setEnabled(false);
  };

  ~Header() override {}

  void paint(juce::Graphics& g) override
  {
    // Paint the border
    auto bounds = getLocalBounds().removeFromTop(getHeight() * 0.5);
    g.setColour(borderColor);
    g.fillRect(bounds);

    // Paint the background
    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);
    g.setColour(backroundColour);
    g.fillRect(bounds);
  }

  void resized() override
  {
    auto bounds = getLocalBounds().removeFromTop(getHeight() * 0.5);
    const auto titleOffset = rawTitleOffset * size;
    const auto titleButtonWidth = rawTitleButtonWidth * size;
    const auto titleBounds = bounds.withWidth(titleButtonWidth)
                               .withCentre(bounds.getCentre())
                               .withY(titleOffset);
    title.setBounds(titleBounds);
    title.setAlwaysOnTop(true);

    // Set the bounds for the outer shadow
    juce::Path outerShadowPath;
    outerShadowPath.addRectangle(bounds);
    outerShadow.setPath(outerShadowPath);
    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

    // Set the bounds for the inner shadow
    juce::Path innerShadowPath;
    innerShadowPath.addRectangle(bounds);
    innerShadow.setPath(innerShadowPath);
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);

    // Set the bounds for the settings button
    const auto settingsButtonBounds =
      Rectangle(bounds).removeFromRight(rawHeaderButtonWidth * size);
    settingsButton.setBounds(settingsButtonBounds);

    // Set the bounds for the title button
    const auto titleButtonBounds =
      bounds.withWidth(titleButtonWidth).withCentre(bounds.getCentre());
    titleButton.setBounds(titleButtonBounds);
  }

  Button& getSettingsButton() noexcept { return settingsButton; }

private:
  Shadow outerShadow;
  Shadow innerShadow;
  Label title;
  Fonts fonts;
  Button settingsButton;
  Button titleButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt