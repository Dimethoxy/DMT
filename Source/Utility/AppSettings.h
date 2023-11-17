#pragma once

#include <JuceHeader.h>

namespace dmt {

struct AppSettings
{
  using Colour = juce::Colour;
  static inline juce::String name = "Neutrino";
  static inline float size = 1.0f;

  static inline struct Colours
  {
    using Colour = juce::Colour;
    static inline Colour background = Colour(25, 26, 33);
    static inline Colour solid_dark = Colour(40, 42, 54);
    static inline Colour solid_mid = Colour(61, 64, 82);
    static inline Colour font = Colour(248, 248, 248);
    static inline Colour primary = Colour(255, 121, 198);
    static inline Colour shadow = Colour(0, 0, 0);
    static inline float opacityStep = 0.05f;
  };

  static inline struct Layout
  {
    static inline float size = 1.0f;
    static inline float margin = 8.0f;
    static inline float leftWidth = 1025.0f;
    static inline float centerWidth = 500.0f;
    static inline float rightWidth = 200.0f;
    static inline float headerHeight = 100.0f;
    static inline float tabHeight = 60.0f;
    static inline float rowHeight = 300.0f;
  };
  static inline struct Panel
  {
    // General
    static inline Colour backgroundColor = Colours::background;
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
    static inline float fontSize = 54.0f;
  };
  static inline struct TriangleButton
  {
    // General
    static inline Colour standbyColour = Colours::shadow.withAlpha(0.5f);
    static inline Colour hoverColour = Colours::shadow.withAlpha(0.1f);
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
  static inline struct Carousel
  {
    static inline float buttonWidth = 80.0f;
    static inline float buttonHeight = 220.0f;
  };
  static inline struct Fonts
  {
    using Typeface = juce::Typeface;

    static inline Typeface::Ptr extraLightTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsExtraLight_ttf,
                                        BinaryData::PoppinsExtraLight_ttfSize);

    static inline Typeface::Ptr regularTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsRegular_ttf,
                                        BinaryData::PoppinsRegular_ttfSize);

    static inline Typeface::Ptr lightTypeface =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsSemiBold_ttf,
        BinaryData::PoppinsSemiBold_ttfSize);

    static inline juce::Font regular = juce::Font(regularTypeface);
  };
};
}
