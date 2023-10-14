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
    static inline bool drawOuterShadow = true;
    static inline bool drawInnerShadow = true;
    static inline bool drawBorder = true;
    static inline bool rawBorderStrength = 3.0f;
    static inline bool borderStrength = 3.0f;
    static inline float cornerSize = 10.0f;
  };
  static inline struct Colours
  {
    using Colour = juce::Colour;
    static inline struct Palette
    {
      static inline Colour background = Colour(32, 32, 32);
      static inline Colour foreground = Colour(250, 250, 250);
      static inline Colour primary = Colour(255, 0, 0);
      static inline Colour shadow = Colour(0, 0, 0);
    };
    static inline Colour background = Palette::background;
    static inline Colour foreground = Palette::foreground;
    static inline Colour border = Palette::foreground;
    static inline Colour outerShadow = Palette::shadow;
    static inline Colour innerShadow = Palette::shadow;
  };
  
  static inline struct Fonts
  {
    static inline juce::Typeface::Ptr regularTypeface =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsRegular_ttf,
        BinaryData::PoppinsRegular_ttfSize);

    static inline juce::Font regular = juce::Font(regularTypeface);
  };
};
}
