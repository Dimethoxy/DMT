#pragma once
//==============================================================================
#include "dmt/gui/widget/HeaderCallbackButton.h"
#include "dmt/gui/widget/HeaderToggleButton.h"
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
  using HeaderCallbackButton = dmt::gui::widget::HeaderCallbackButton;
  using HeaderToggleButton = dmt::gui::widget::HeaderToggleButton;
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
  Header(juce::String titleText, AudioProcessorValueTreeState& _apvts)
    : outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
    , title(String("ProjectLabel"),
            fonts.display,
            titleFontSize,
            titleFontColour,
            juce::Justification::centred)
    , settingsButton("HeaderSettingsButton", "Settings")
    , settingsExitButton("HeaderSettingsExitButton", "Back")
    , hideHeaderButton("HeaderHideButton", "HideHeader")
    , titleButton("HeaderTitleButton", "None")
    , updateButton("UpdateButton", "Download")
    , bypassButton("BypassButton", "Bypass", "Bypass", _apvts)
    , presetsButton("PresetsButton", "Presets")
  {
    // Shadows
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);

    // Title
    addAndMakeVisible(title);
    addAndMakeVisible(titleButton);
    title.setText(titleText);
    titleButton.setEnabled(false);

    // Settings Button
    addAndMakeVisible(settingsButton);
    addAndMakeVisible(settingsExitButton);
    settingsExitButton.setVisible(false);
    addAndMakeVisible(hideHeaderButton);

    // These buttons are not done yet so we hide them for now
    addAndMakeVisible(updateButton);
    updateButton.setVisible(false);
    addAndMakeVisible(bypassButton);
    bypassButton.setVisible(false);
    addAndMakeVisible(presetsButton);
    presetsButton.setVisible(false);
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

    // Set the bounds for the update download button
    const auto updateButtonBounds =
      buttonAreaBounds.removeFromRight(rawHeaderButtonWidth * size);
    updateButton.setBounds(updateButtonBounds);

    // Set the bounds for the bypass button
    const auto bypassButtonBounds =
      buttonAreaBounds.removeFromLeft(rawHeaderButtonWidth * size);
    bypassButton.setBounds(bypassButtonBounds);

    // Set the bounds for the presets button
    const auto presetsButtonBounds =
      buttonAreaBounds.removeFromLeft(rawHeaderButtonWidth * size);
    presetsButton.setBounds(presetsButtonBounds);

    // Set the bounds for the title button
    const auto titleButtonBounds =
      bounds.withWidth(titleButtonWidth).withCentre(bounds.getCentre());
    titleButton.setBounds(titleButtonBounds);
  }

  HeaderCallbackButton& getSettingsButton() noexcept { return settingsButton; }
  HeaderCallbackButton& getSettingsExitButton() noexcept
  {
    return settingsExitButton;
  }
  HeaderCallbackButton& getHideHeaderButton() noexcept
  {
    return hideHeaderButton;
  }

private:
  Shadow outerShadow;
  Shadow innerShadow;
  Label title;
  Fonts fonts;
  HeaderCallbackButton settingsButton;
  HeaderCallbackButton settingsExitButton;
  HeaderCallbackButton hideHeaderButton;
  HeaderCallbackButton titleButton;
  HeaderCallbackButton updateButton;
  HeaderToggleButton bypassButton;
  HeaderCallbackButton presetsButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt