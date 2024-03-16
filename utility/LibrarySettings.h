//==============================================================================
#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
//==============================================================================
struct LibrarySettings
{
  using Colour = juce::Colour;
  static inline const bool debugBounds = false;
  static inline const bool debugGrid = false;
  static inline const float macFontScale = 0.9f;
  static inline int fps = 60;

  struct Colours
  {
    using Colour = juce::Colour;
    static inline Colour background = Colour(25, 26, 33);
    static inline Colour solid_dark = Colour(40, 42, 54);
    static inline Colour solid_mid = Colour(61, 64, 82);
    static inline Colour font = Colour(248, 248, 248);
    static inline Colour primary = Colour(80, 250, 123);
    static inline Colour shadow = Colour(0, 0, 0);
    static inline float opacityStep = 0.05f;
  };

  struct Layout
  {
    static inline float size = 1.0f;
    static inline float margin = 10.0f;
  };

  struct Panel
  {
    // General
    static inline float padding = 12.0f;
    static inline float cornerSize = 15.0f;
    // Border
    static inline bool drawBorder = true;
    static inline Colour borderColour = Colours::solid_dark;
    static inline float borderStrength = 4.0f;
    static inline juce::Colour backgroundColour = Colours::solid_mid;
    // Shadows
    static inline bool drawOuterShadow = true;
    static inline bool drawInnerShadow = false;
    static inline Colour outerShadowColour = Colours::shadow;
    static inline Colour innerShadowColour = Colours::shadow;
    static inline float outerShadowRadius = 10.0f;
    static inline float innerShadowRadius = 10.0f;
    // Text
    static inline juce::Colour fontColor = Colours::font;
    static inline float fontSize = 34.0f;
  };

  struct Slider
  {
    // General
    static inline float padding = 8.0f;
    static inline float baseWidth = 105.0f;
    static inline float baseHeight = 119.0f;
    static inline float sliderSize = 0.80f;
    static inline float labelsSize = 0.32f;
    // Font
    static inline Colour titleFontColour = Colours::font;
    static inline Colour infoFontColour = Colours::font;
    static inline float titleFontSize = 24.0f;
    static inline float infoFontSize = 18.0f;
    // Shaft
    static inline juce::Colour shaftColour = Colours::font;
    static inline float shaftLineStrength = 3.0f;
    static inline float shaftSize = 0.45f;
    // Rail
    static inline juce::Colour lowerRailColour = Colours::solid_dark;
    static inline juce::Colour upperRailColour = Colours::primary;
    static inline float railWidth = 8.0f;
    static inline float railSize = 0.72f;
    // Thumb
    static inline juce::Colour thumbInnerColour = Colours::solid_dark;
    static inline juce::Colour thumOuterColour = Colours::font;
    static inline float thumbSize = 18.0f;
    static inline float thumbStrength = 3.0f;
    // Selections
    static inline juce::Colour selectionOuterColour = Colours::font;
    static inline juce::Colour selectionInnerColour = Colours::solid_dark;
    static inline juce::Colour selectionActiveColour = Colours::primary;
    static inline float selectionWidth = 2.0f;
    static inline float selectionSize = 9.0f;
    static inline float selectionActivePadding = 2.0f;
  };
  struct TriangleButton
  {
    // General
    static inline Colour standbyColour = Colours::shadow.withAlpha(0.5f);
    static inline Colour hoverColour = Colours::font;
    static inline float margin = 15.0f;
    static inline float toggleReduction = 0.9f;
    // Border
    static inline bool drawBorder = true;
    static inline Colour borderColour = Colours::primary;
    static inline float borderStrength = 4.0f;
    // Shadows
    static inline bool drawOuterShadow = false;
    static inline bool drawInnerShadow = true;
    static inline Colour outerShadowColour = Colours::shadow;
    static inline Colour innerShadowColour = Colours::primary;
    static inline float outerShadowRadius = 4.0f;
    static inline float innerShadowRadius = 4.0f;
  };
  struct OscillatorDisplay
  {
    // General
    static inline int resolution = 256;
    // Shadow
    static inline bool drawOuterShadow = false;
    static inline bool drawInnerShadow = true;
    static inline Colour outerShadowColour = Colours::shadow;
    static inline Colour innerShadowColour = Colours::primary;
    static inline float outerShadowRadius = 4.0f;
    static inline float innerShadowRadius = 4.0f;
  };
  struct Oscilloscope
  {
    // General
    static inline Colour backgroundColour = Colours::background;
    static inline float padding = 10.0f;
    static inline float cornerSize = 15.0f;
    // Border
    static inline bool drawBorder = true;
    static inline Colour borderColour = Colours::solid_dark;
    static inline float borderStrength = 4.0f;
    // Shadow
    static inline bool drawOuterShadow = false;
    static inline bool drawInnerShadow = true;
    static inline Colour outerShadowColour = Colours::shadow;
    static inline Colour innerShadowColour = Colours::shadow;
    static inline float outerShadowRadius = 4.0f;
    static inline float innerShadowRadius = 4.0f;
  };
  struct Carousel
  {
    static inline float buttonWidth = 60.0f;
    static inline float buttonHeight = 180.0f;
  };
  struct Fonts
  {
    using Typeface = juce::Typeface;
    using Font = juce::Font;

    static inline Typeface::Ptr thinTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsThin_ttf,
                                        BinaryData::PoppinsThin_ttfSize);
    static inline Font thin = Font(thinTypeface);

    static inline Typeface::Ptr extraLightTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsExtraLight_ttf,
                                        BinaryData::PoppinsExtraLight_ttfSize);
    static inline Font extraLight = Font(extraLightTypeface);

    static inline Typeface::Ptr lightTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsLight_ttf,
                                        BinaryData::PoppinsLight_ttfSize);
    static inline Font light = Font(lightTypeface);

    static inline Typeface::Ptr regularTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsRegular_ttf,
                                        BinaryData::PoppinsRegular_ttfSize);
    static inline Font regular = Font(regularTypeface);

    static inline Typeface::Ptr mediumTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsMedium_ttf,
                                        BinaryData::PoppinsMedium_ttfSize);
    static inline Font medium = Font(mediumTypeface);

    static inline Typeface::Ptr semiBoldTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsSemiBold_ttf,
                                        BinaryData::PoppinsSemiBold_ttfSize);
    static inline Font semiBold = Font(semiBoldTypeface);

    static inline Typeface::Ptr boldTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsBold_ttf,
                                        BinaryData::PoppinsBold_ttfSize);
    static inline Font bold = Font(boldTypeface);

    static inline Typeface::Ptr extraBoldTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsExtraBold_ttf,
                                        BinaryData::PoppinsExtraBold_ttfSize);
    static inline Font extraBold = Font(extraBoldTypeface);

    static inline Typeface::Ptr blackTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsBlack_ttf,
                                        BinaryData::PoppinsBlack_ttfSize);
    static inline Font black = Font(blackTypeface);
  };
};
} // namespace dmt