#pragma once

#include <JuceHeader.h>

namespace dmt {

struct AppSettings
{
  using Colour = juce::Colour;
  static inline juce::String name = "Neutrino";
  static inline float size = 1.0f;
  
  static inline struct Layout
  {
    static inline float size = 1.0f;
    static inline float margin = 10.0f;
    static inline float leftWidth = 1025.0f;
    static inline float centerWidth = 500.0f;
    static inline float rightWidth = 200.0f;
    static inline float headerHeight = 100.0f;
    static inline float tabHeight = 60.0f;
    static inline float rowHeight = 300.0f; 
  };
  static inline struct Appearance
  {
    static inline bool drawOuterShadow = false;
    static inline bool drawInnerShadow = false;
        static inline bool drawBorder = true;
    static inline float borderStrength = 4.0f;
    static inline float cornerSize = 0.0f;
  };
  static inline struct Colours
  {
    using Colour = juce::Colour;
    static inline struct Palette
    {
      static inline Colour background = Colour(18, 18, 18);
      static inline Colour foreground = Colour(250, 250, 250);
      static inline Colour primary = Colour(255, 0, 0);
      static inline Colour shadow = Colour(0, 0, 0);
      static inline float opacityStep = 0.05f; 
    };
    static inline Colour background = Palette::background;
    static inline Colour foreground = Palette::foreground;
    static inline Colour solidDark = Palette::background.withLightness(
      Palette::background.getLightness() + Palette::opacityStep * 1);
    static inline Colour solidMid = Palette::background.withLightness(
      Palette::background.getLightness() + Palette::opacityStep * 2);
    static inline Colour outerShadow = Palette::shadow;
    static inline Colour innerShadow = Palette::shadow;
  };
  
  static inline struct Fonts
  {
    using Typeface = juce::Typeface;
    static inline float panelTitleSize = 52.0f;
    
    static inline Typeface::Ptr extraLightTypeface =
    Typeface::createSystemTypefaceFor(
            BinaryData::PoppinsExtraLight_ttf,
            BinaryData::PoppinsExtraLight_ttfSize);

    static inline Typeface::Ptr regularTypeface =
      Typeface::createSystemTypefaceFor(
          BinaryData::PoppinsRegular_ttf,
          BinaryData::PoppinsRegular_ttfSize);

    static inline Typeface::Ptr lightTypeface =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsSemiBold_ttf,
        BinaryData::PoppinsSemiBold_ttfSize);

    static inline juce::Font regular = juce::Font(regularTypeface);
    
  };
};
}
