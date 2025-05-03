//==============================================================================
#pragma once
//==============================================================================
#include "BinaryData.h"
#include "configuration/Container.h"
#include <JuceHeader.h>
//==============================================================================
#ifndef OS_IS_WINDOWS
#define OS_IS_WINDOWS 0
#endif
#ifndef OS_IS_DARWIN
#define OS_IS_DARWIN 0
#endif
#ifndef OS_IS_LINUX
#define OS_IS_LINUX 0
#endif
#ifndef DMT_DISABLE_UPDATE_NOTIFICATION
#define DMT_DISABLE_UPDATE_NOTIFICATION 0
#endif
//==============================================================================
namespace dmt {
//==============================================================================
struct Settings
{
public:
  using Colour = juce::Colour;

  Settings() = delete;
  Settings(const Settings& obj) = delete;
  ~Settings() = delete;

  static inline dmt::configuration::Container container;

  static inline auto appName = juce::String(""); // TODO: Remove this

  static inline auto& framerate =
    container.add<int>("General.TargetFramerate", 30);
  static inline auto& unixFontScalingFactor =
    container.add<float>("General.UnixFontScalingFactor", 0.9f);
  static inline auto& debugBounds =
    container.add<bool>("General.ShowDebugBounds", false);
  static inline auto& debugGrid =
    container.add<bool>("General.ShowDebugGrid", false);
  static inline auto& displayUpdateNotifications =
    container.add<bool>("General.DisplayUpdateNotifications", true);
  static inline auto& themeVersion =
    container.add<int>("General.ThemeVersion", 1);

private:
  struct Colours
  {
    using Colour = juce::Colour;
    static inline Colour background = Colour(25, 26, 33);
    static inline Colour solidDark = Colour(40, 42, 54);
    static inline Colour solidMedium = Colour(61, 64, 82);
    static inline Colour solidLight = Colour(69, 73, 94);
    static inline Colour solidExtraLight = Colour(78, 83, 106);
    static inline Colour font = Colour(248, 248, 248);
    static inline Colour primary = Colour(80, 250, 123);
    static inline Colour shadow = Colour(0, 0, 0);
  };

public:
  struct Window
  {
    static inline auto& size = container.add<float>("Window.Size", 1.0f);
    static inline auto& margin = container.add<float>("Window.Margin", 10.0f);
    static inline auto& backgroundColour =
      container.add<Colour>("Window.BackgroundColour", Colours::background);
  };

  struct Header
  {
    static inline auto& backgroundColour =
      container.add<Colour>("Header.BackgroundColour", Colours::solidMedium);
    static inline auto& borderColor =
      container.add<Colour>("Header.BorderColour", Colours::solidDark);
    static inline auto& titleColour =
      container.add<Colour>("Header.TitleColour", Colours::font);
    static inline auto& buttonColour =
      container.add<Colour>("Header.ButtonColour", Colours::solidDark);
    static inline auto& titleFontColour =
      container.add<Colour>("Header.TitleFontColour", Colours::font);
    static inline auto& titleFontSize =
      container.add<float>("Header.TitleFontSize", 34.0f);
    static inline auto& titleOffset =
      container.add<float>("Header.TitleOffset", 2.5f);
    static inline auto& titleButtonWidth =
      container.add<float>("Header.TitleButtonWidth", 120.0f);
    static inline auto& buttonWidth =
      container.add<float>("Header.ButtonWidth", 48.0f);
    static inline auto& height = container.add<int>("Header.Height", 50);
    static inline auto& borderButtonBackgroundColour =
      container.add<Colour>("Header.BorderButtonBackgroundColour",
                            Colours::primary);
    static inline auto& borderButtonFontColour =
      container.add<Colour>("Header.BorderButtonFontColour", Colours::shadow);
    static inline auto& borderButtonHeight =
      container.add<int>("Header.BorderButtonHeight", 25);
  };

  struct Popover
  {
    static inline auto& backgroundColour =
      container.add<Colour>("Popover.BackgroundColour", Colours::solidDark);
    static inline auto& borderColour =
      container.add<Colour>("Popover.BorderColour", Colours::font);
    static inline auto& textColour =
      container.add<Colour>("Popover.TextColour", Colours::font);
    static inline auto& innerShadowColour =
      container.add<Colour>("Popover.InnerShadowColour", Colours::shadow);
    static inline auto& outerShadowColour =
      container.add<Colour>("Popover.OuterShadowColour", Colours::shadow);
    static inline auto& titleFontColour =
      container.add<Colour>("Popover.TitleFontColour", Colours::font);
    static inline auto& messageFontColour =
      container.add<Colour>("Popover.MessageFontColour", Colours::font);
    static inline auto& innerShadowRadius =
      container.add<float>("Popover.InnerShadowRadius", 10.0f);
    static inline auto& outerShadowRadius =
      container.add<float>("Popover.OuterShadowRadius", 10.0f);
    static inline auto& rawCornerRadius =
      container.add<float>("Popover.CornerRadius", 10.0f);
    static inline auto& rawBorderWidth =
      container.add<float>("Popover.BorderWidth", 2.0f);
    static inline auto& titleFontSize =
      container.add<float>("Popover.TitleFontSize", 24.0f);
    static inline auto& messageFontSize =
      container.add<float>("Popover.MessageFontSize", 18.0f);
    static inline auto& drawOuterShadow =
      container.add<bool>("Popover.DrawOuterShadow", true);
    static inline auto& drawInnerShadow =
      container.add<bool>("Popover.DrawInnerShadow", true);
  };

  struct Tooltip
  {
    static inline auto& backgroundColour =
      container.add<Colour>("Tooltip.BackgroundColour", Colours::solidDark);
    static inline auto& borderColour =
      container.add<Colour>("Tooltip.BorderColour", Colours::font);
    static inline auto& fontColour =
      container.add<Colour>("Tooltip.FontColour", Colours::font);
    static inline auto& innerShadowColour =
      container.add<Colour>("Tooltip.InnerShadowColour", Colours::shadow);
    static inline auto& outerShadowColour =
      container.add<Colour>("Tooltip.OuterShadowColour", Colours::shadow);
    static inline auto& cornerRadius =
      container.add<float>("Tooltip.CornerRadius", 10.0f);
    static inline auto& borderWidth =
      container.add<float>("Tooltip.BorderWidth", 2.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("Tooltip.InnerShadowRadius", 10.0f);
    static inline auto& outerShadowRadius =
      container.add<float>("Tooltip.OuterShadowRadius", 10.0f);
    static inline auto& fontSize =
      container.add<float>("Tooltip.FontSize", 20.0f);
    static inline auto& textHorizontalPadding =
      container.add<float>("Tooltip.TextHorizontalPadding", 10.0f);
    static inline auto& textVerticalPadding =
      container.add<float>("Tooltip.TextVerticalPadding", 5.0f);
    static inline auto& drawOuterShadow =
      container.add<bool>("Tooltip.DrawOuterShadow", true);
    static inline auto& drawInnerShadow =
      container.add<bool>("Tooltip.DrawInnerShadow", true);
  };

  struct Alerts
  {
    static inline auto& warningBackgroundColour =
      container.add<Colour>("Alert.WarningBackgroundColour",
                            Colours::solidDark);
    static inline auto& warningBorderColour =
      container.add<Colour>("Alert.WarningBorderColour", Colours::font);
    static inline auto& warningFontColour =
      container.add<Colour>("Alert.WarningFontColour", Colours::font);
    static inline auto& warningIconColour =
      container.add<Colour>("Alert.WarningIconColour", Colours::font);
    static inline auto& warningOuterShadowColour =
      container.add<Colour>("Alert.WarningOuterShadowColour", Colours::shadow);
    static inline auto& warningInnerShadowColour =
      container.add<Colour>("Alert.WarningInnerShadowColour", Colours::shadow);

    static inline auto& errorBackgroundColour =
      container.add<Colour>("Alert.ErrorBackgroundColour", Colours::solidDark);
    static inline auto& errorBorderColour =
      container.add<Colour>("Alert.ErrorBorderColour", Colours::font);
    static inline auto& errorFontColour =
      container.add<Colour>("Alert.ErrorFontColour", Colours::font);
    static inline auto& errorIconColour =
      container.add<Colour>("Alert.ErrorIconColour", Colours::font);
    static inline auto& errorOuterShadowColour =
      container.add<Colour>("Alert.ErrorOuterShadowColour", Colours::shadow);
    static inline auto& errorInnerShadowColour =
      container.add<Colour>("Alert.ErrorInnerShadowColour", Colours::shadow);

    static inline auto& infoBackgroundColour =
      container.add<Colour>("Alert.InfoBackgroundColour", Colours::solidDark);
    static inline auto& infoBorderColour =
      container.add<Colour>("Alert.InfoBorderColour", Colours::font);
    static inline auto& infoFontColour =
      container.add<Colour>("Alert.InfoFontColour", Colours::font);
    static inline auto& infoIconColour =
      container.add<Colour>("Alert.InfoIconColour", Colours::font);
    static inline auto& infoOuterShadowColour =
      container.add<Colour>("Alert.InfoOuterShadowColour", Colours::shadow);
    static inline auto& infoInnerShadowColour =
      container.add<Colour>("Alert.InfoInnerShadowColour", Colours::shadow);

    static inline auto& successBackgroundColour =
      container.add<Colour>("Alert.SuccessBackgroundColour",
                            Colours::solidDark);
    static inline auto& successBorderColour =
      container.add<Colour>("Alert.SuccessBorderColour", Colours::font);
    static inline auto& successFontColour =
      container.add<Colour>("Alert.SuccessFontColour", Colours::font);
    static inline auto& successIconColour =
      container.add<Colour>("Alert.SuccessIconColour", Colours::font);
    static inline auto& successOuterShadowColour =
      container.add<Colour>("Alert.SuccessOuterShadowColour", Colours::shadow);
    static inline auto& successInnerShadowColour =
      container.add<Colour>("Alert.SuccessInnerShadowColour", Colours::shadow);

    static inline auto& cornerRadius =
      container.add<float>("Alert.CornerRadius", 10.0f);
    static inline auto& borderWidth =
      container.add<float>("Alert.BorderWidth", 2.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("Alert.InnerShadowRadius", 10.0f);
    static inline auto& outerShadowRadius =
      container.add<float>("Alert.OuterShadowRadius", 10.0f);
    static inline auto& titleFontSize =
      container.add<float>("Alert.TitleFontSize", 18.0f);
    static inline auto& messageFontSize =
      container.add<float>("Alert.MessageFontSize", 14.0f);
    static inline auto& textHorizontalPadding =
      container.add<float>("Alert.TextHorizontalPadding", 10.0f);
    static inline auto& textVerticalPadding =
      container.add<float>("Alert.TextVerticalPadding", 5.0f);
    static inline auto& maxAge = container.add<float>("Alert.MaxAge", 1.0f);
    static inline auto& fadeOutTime =
      container.add<float>("Alert.FadeOutTime", 0.5f);
    static inline auto& iconSize =
      container.add<float>("Alert.IconSize", 20.0f);
    static inline auto& contentSpacing =
      container.add<float>("Alert.ContentSpacing", 8.0f);

    static inline auto& drawOuterShadow =
      container.add<bool>("Alert.DrawOuterShadow", true);
    static inline auto& drawInnerShadow =
      container.add<bool>("Alert.DrawInnerShadow", true);
    static inline auto& alertWidth =
      container.add<int>("Alert.AlertWidth", 250);
    static inline auto& alertHeight =
      container.add<int>("Alert.AlertHeight", 75);
  };

  struct Button
  {
    static inline auto& backgroundColour =
      container.add<Colour>("Button.BackgroundColour", Colours::solidDark);
    static inline auto& outerShadowColour =
      container.add<Colour>("Button.OuterShadowColour",
                            Colours::shadow.withAlpha(0.5f));
    static inline auto& innerShadowColour =
      container.add<Colour>("Button.InnerShadowColour",
                            Colours::shadow.withAlpha(0.5f));
    static inline auto& fontColour =
      container.add<Colour>("Button.FontColour", Colours::font);
    static inline auto& hoverColour =
      container.add<Colour>("Button.HoverColour", Colours::primary);
    static inline auto& clickColour =
      container.add<Colour>("Button.ClickColour", Colours::font);
    static inline auto& outerShadowRadius =
      container.add<float>("Button.OuterShadowRadius", 5.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("Button.InnerShadowRadius", 5.0f);
    static inline auto& cornerRadius =
      container.add<float>("Button.CornerRadius", 10.0f);
    static inline auto& padding = container.add<float>("Button.Padding", 6.0f);
    static inline auto& drawInnerShadow =
      container.add<bool>("Button.DrawInnerShadow", true);
    static inline auto& drawOuterShadow =
      container.add<bool>("Button.DrawOuterShadow", false);
  };

  struct Panel
  {
    static inline auto& padding = container.add<float>("Panel.Padding", 10.0f);
    static inline auto& cornerSize =
      container.add<float>("Panel.CornerSize", 15.0f);
    static inline auto& drawBorder =
      container.add<bool>("Panel.DrawBorder", true);
    static inline auto& borderColour =
      container.add<Colour>("Panel.BorderColour", Colours::solidDark);
    static inline auto& borderStrength =
      container.add<float>("Panel.BorderStrength", 4.0f);
    static inline auto& backgroundColour =
      container.add<Colour>("Panel.BackgroundColour", Colours::solidMedium);
    static inline auto& drawOuterShadow =
      container.add<bool>("Panel.DrawOuterShadow", true);
    static inline auto& drawInnerShadow =
      container.add<bool>("Panel.DrawInnerShadow", false);
    static inline auto& outerShadowColour =
      container.add<Colour>("Panel.OuterShadowColour", Colours::shadow);
    static inline auto& innerShadowColour =
      container.add<Colour>("Panel.InnerShadowColour", Colours::shadow);
    static inline auto& outerShadowRadius =
      container.add<float>("Panel.OuterShadowRadius", 10.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("Panel.InnerShadowRadius", 10.0f);
    static inline auto& fontColor =
      container.add<Colour>("Panel.FontColor", Colours::font);
    static inline auto& fontSize =
      container.add<float>("Panel.FontSize", 34.0f);
  };

  struct Carousel
  {
    static inline auto& buttonWidth =
      container.add<float>("Carousel.ButtonWidth", 60.0f);
    static inline auto& buttonHeight =
      container.add<float>("Carousel.ButtonHeight", 180.0f);
  };

  struct SettingsEditor
  {
    static inline auto& fontColour =
      container.add<Colour>("SettingsEditor.FontColour", Colours::font);
    static inline auto& selectedFontColour =
      container.add<Colour>("SettingsEditor.SelectedFontColour",
                            juce::Colours::black);
    static inline auto& selectedLabelBackgroundColour =
      container.add<Colour>("SettingsEditor.SelectedLabelBackgroundColour",
                            Colours::primary);
    static inline auto& seperatorColour =
      container.add<Colour>("SettingsEditor.SeperatorColour",
                            Colours::solidDark);
    static inline auto& scrollBarColour =
      container.add<Colour>("SettingsEditor.ScrollBarColour", Colours::font);
    static inline auto& scrollBarBackgroundColour =
      container.add<Colour>("SettingsEditor.ScrollBarBackgroundColour",
                            juce::Colours::transparentBlack);
    static inline auto& padding =
      container.add<float>("SettingsEditor.Padding", 5.0f);
    static inline auto& fontSize =
      container.add<float>("SettingsEditor.FontSize", 17.0f);
    static inline auto& scrollBarThickness =
      container.add<float>("SettingsEditor.ScrollBarThickness", 8.0f);
    static inline auto& labelHorizontalPadding =
      container.add<float>("SettingsEditor.LabelHorizontalPadding", 10.0f);
  };

  struct Slider
  {
    // General
    static inline auto& padding = container.add<float>("Slider.Padding", 8.0f);
    static inline auto& baseWidth =
      container.add<float>("Slider.BaseWidth", 105.0f);
    static inline auto& baseHeight =
      container.add<float>("Slider.BaseHeight", 119.0f);
    static inline auto& sliderSize =
      container.add<float>("Slider.SliderSize", 0.80f);
    static inline auto& labelsSize =
      container.add<float>("Slider.LabelsSize", 0.32f);
    // Font
    static inline auto& titleFontColour =
      container.add<Colour>("Slider.TitleFontColour", Colours::font);
    static inline auto& infoFontColour =
      container.add<Colour>("Slider.InfoFontColour", Colours::font);
    static inline auto& titleFontSize =
      container.add<float>("Slider.TitleFontSize", 24.0f);
    static inline auto& infoFontSize =
      container.add<float>("Slider.InfoFontSize", 18.0f);
    // Shaft
    static inline auto& shaftColour =
      container.add<Colour>("Slider.ShaftColour", Colours::font);
    static inline auto& shaftLineStrength =
      container.add<float>("Slider.ShaftLineStrength", 3.0f);
    static inline auto& shaftSize =
      container.add<float>("Slider.ShaftSize", 0.45f);
    // Rail
    static inline auto& lowerRailColour =
      container.add<Colour>("Slider.LowerRailColour", Colours::solidDark);
    static inline auto& upperRailColour =
      container.add<Colour>("Slider.UpperRailColour", Colours::primary);
    static inline auto& railWidth =
      container.add<float>("Slider.RailWidth", 8.0f);
    static inline auto& railSize =
      container.add<float>("Slider.RailSize", 0.72f);
    // Thumb
    static inline auto& thumbInnerColour =
      container.add<Colour>("Slider.ThumbInnerColour", Colours::solidDark);
    static inline auto& thumbOuterColour =
      container.add<Colour>("Slider.ThumbOuterColour", Colours::font);
    static inline auto& thumbSize =
      container.add<float>("Slider.ThumbSize", 22.0f);
    static inline auto& thumbStrength =
      container.add<float>("Slider.ThumbStrength", 3.0f);
    // Selections
    static inline auto& selectionOuterColour =
      container.add<Colour>("Slider.SelectionOuterColour", Colours::font);
    static inline auto& selectionInnerColour =
      container.add<Colour>("Slider.SelectionInnerColour", Colours::solidDark);
    static inline auto& selectionActiveColour =
      container.add<Colour>("Slider.SelectionActiveColour", Colours::primary);
    static inline auto& selectionWidth =
      container.add<float>("Slider.SelectionWidth", 2.0f);
    static inline auto& selectionSize =
      container.add<float>("Slider.SelectionSize", 9.0f);
    static inline auto& selectionActivePadding =
      container.add<float>("Slider.SelectionActivePadding", 2.0f);
  };

  struct Display
  {
    // General
    static inline auto& backgroundColour =
      container.add<Colour>("Display.BackgroundColour", Colours::background);
    // Layout
    static inline auto& padding =
      container.add<float>("Display.Padding", 10.0f);
    static inline auto& cornerSize =
      container.add<float>("Display.CornerSize", 8.0f);
    // Border
    static inline auto& drawBorder =
      container.add<bool>("Display.DrawBorder", true);
    static inline auto& borderColour =
      container.add<Colour>("Display.BorderColour", Colours::solidDark);
    static inline auto& borderStrength =
      container.add<float>("Display.BorderStrength", 4.0f);
    // Shadow
    static inline auto& drawOuterShadow =
      container.add<bool>("Display.DrawOuterShadow", false);
    static inline auto& drawInnerShadow =
      container.add<bool>("Display.DrawInnerShadow", true);
    static inline auto& outerShadowColour =
      container.add<Colour>("Display.OuterShadowColour", Colours::shadow);
    static inline auto& innerShadowColour =
      container.add<Colour>("Display.InnerShadowColour", Colours::shadow);
    static inline auto& outerShadowRadius =
      container.add<float>("Display.OuterShadowRadius", 4.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("Display.InnerShadowRadius", 4.0f);
  };

  struct TriangleButton
  {
    // General
    static inline auto& standbyColour =
      container.add<Colour>("TriangleButton.StandbyColour",
                            Colours::shadow.withAlpha(0.5f));
    static inline auto& hoverColour =
      container.add<Colour>("TriangleButton.HoverColour", Colours::font);
    static inline auto& margin =
      container.add<float>("TriangleButton.Margin", 15.0f);
    static inline auto& toggleReduction =
      container.add<float>("TriangleButton.ToggleReduction", 0.9f);
    // Border
    static inline auto& drawBorder =
      container.add<bool>("TriangleButton.DrawBorder", true);
    static inline auto& borderColour =
      container.add<Colour>("TriangleButton.BorderColour", Colours::primary);
    static inline auto& borderStrength =
      container.add<float>("TriangleButton.BorderStrength", 4.0f);
    // Shadows
    static inline auto& drawOuterShadow =
      container.add<bool>("TriangleButton.DrawOuterShadow", false);
    static inline auto& drawInnerShadow =
      container.add<bool>("TriangleButton.DrawInnerShadow", true);
    static inline auto& outerShadowColour =
      container.add<Colour>("TriangleButton.OuterShadowColour",
                            Colours::shadow);
    static inline auto& innerShadowColour =
      container.add<Colour>("TriangleButton.InnerShadowColour",
                            Colours::primary);
    static inline auto& outerShadowRadius =
      container.add<float>("TriangleButton.OuterShadowRadius", 4.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("TriangleButton.InnerShadowRadius", 4.0f);
  };

  struct OscillatorDisplay
  {
    // General
    static inline auto& resolution =
      container.add<int>("OscillatorDisplay.Resolution", 256);
    // Shadow
    static inline auto& drawOuterShadow =
      container.add<bool>("OscillatorDisplay.DrawOuterShadow", false);
    static inline auto& drawInnerShadow =
      container.add<bool>("OscillatorDisplay.DrawInnerShadow", true);
    static inline auto& outerShadowColour =
      container.add<Colour>("OscillatorDisplay.OuterShadowColour",
                            Colours::shadow);
    static inline auto& innerShadowColour =
      container.add<Colour>("OscillatorDisplay.InnerShadowColour",
                            Colours::primary);
    static inline auto& outerShadowRadius =
      container.add<float>("OscillatorDisplay.OuterShadowRadius", 4.0f);
    static inline auto& innerShadowRadius =
      container.add<float>("OscillatorDisplay.InnerShadowRadius", 4.0f);
  };

  struct Oscilloscope
  {
    // General
    static inline auto& defaultZoom =
      container.add<float>("Oscilloscope.DefaultZoom", 25.0f);
    static inline auto& defaultGain =
      container.add<float>("Oscilloscope.DefaultGain", 0.0f);
    static inline auto& defaultThickness =
      container.add<float>("Oscilloscope.DefaultThickness", 3.0f);
  };
};

} // namespace dmt