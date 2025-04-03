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
  const Colour& backgroundColour = HeaderSettings::backgroundColour;
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
            Colours::white, // TODO: Change this to a title colour
            juce::Justification::centred)
    , settingsButton("HeaderSettingsButton", "Settings")
    , settingsExitButton("HeaderSettingsExitButton", "Back")
    , hideHeaderButton("HeaderHideButton", "HideHeader")
    , titleButton("HeaderTitleButton", "None")
    , bypassButton("BypassButton", "Bypass")
  {
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
    addAndMakeVisible(title);
    addAndMakeVisible(settingsButton);
    addAndMakeVisible(settingsExitButton);
    addAndMakeVisible(hideHeaderButton);
    addAndMakeVisible(titleButton);
    addAndMakeVisible(bypassButton);
    title.setText(titleText);
    titleButton.setEnabled(false);
    settingsExitButton.setVisible(false);
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
    g.setColour(backgroundColour);
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

    // Set the bounds for the inner header
    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);

    // Header Button Bounds
    const int headerButtonAmount = 2;
    const auto headerButtonAreaWidth =
      rawHeaderButtonWidth * size * headerButtonAmount;
    const auto buttonAreaPadding = 2.0 * rawButtonPadding * size;
    auto buttonAreaBounds =
      bounds.withWidth(bounds.getWidth() - buttonAreaPadding)
        .withCentre(bounds.getCentre());

    // Set the bounds for the settings button
    const auto settingsButtonBounds =
      buttonAreaBounds.removeFromRight(rawHeaderButtonWidth * size);
    settingsButton.setBounds(settingsButtonBounds);
    settingsExitButton.setBounds(settingsButtonBounds);

    // Set the bounds for the hide header button
    const auto hideHeaderButtonBounds =
      buttonAreaBounds.removeFromRight(rawHeaderButtonWidth * size);
    hideHeaderButton.setBounds(hideHeaderButtonBounds);

    // Set the bounds for the bypass button
    const auto bypassButtonBounds =
      buttonAreaBounds.removeFromLeft(rawHeaderButtonWidth * size);
    bypassButton.setBounds(bypassButtonBounds);

    // Set the bounds for the title button
    const auto titleButtonBounds =
      bounds.withWidth(titleButtonWidth).withCentre(bounds.getCentre());
    titleButton.setBounds(titleButtonBounds);
  }

  Button& getSettingsButton() noexcept { return settingsButton; }
  Button& getSettingsExitButton() noexcept { return settingsExitButton; }
  Button& getHideHeaderButton() noexcept { return hideHeaderButton; }

private:
  Shadow outerShadow;
  Shadow innerShadow;
  Label title;
  Fonts fonts;
  Button settingsButton;
  Button settingsExitButton;
  Button hideHeaderButton;
  Button titleButton;
  Button bypassButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt