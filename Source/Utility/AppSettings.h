#pragma once

#include <JuceHeader.h>

namespace dmt {

struct AppSettings
{
  static inline juce::String name = "Neutrino";
  static inline struct Colours
  {
    static inline juce::Colour background = juce::Colour(18, 18, 18);
    static inline juce::Colour foreground = juce::Colour(29, 29, 29);
    static inline juce::Colour panelBorder = juce::Colour(0, 0, 0);
    static inline juce::Colour shadow = juce::Colour(0, 0, 0);
  };
  static inline struct Fonts
  {
    static inline juce::Typeface::Ptr regularTypeface =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsRegular_ttf,
        BinaryData::PoppinsRegular_ttfSize);

    static inline juce::Font regular = juce::Font(regularTypeface);

    static inline juce::Typeface::Ptr mediumTypeface =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsMedium_ttf,
        BinaryData::PoppinsMedium_ttfSize);

    static inline juce::Font medium = juce::Font(mediumTypeface);
  };
  static inline struct FontSizes
  {
    static inline int sliderValueLabel = 24;
    static inline int sliderNameLabel = 32;
    static inline int editorTitle = 48;
    static inline int triggerLabel = 30;
  };
  static inline struct Panel
  {
    static inline float margin = 20.0f;
    static inline float outerCornerSize = 10.0f;
    static inline float innerCornerSize = 9.0f;
    static inline float borderSize = 2.0f;
    static inline auto backgroundColour = Colours::background;
    static inline auto foregroundColour = Colours::foreground;
    static inline auto borderColour = Colours::panelBorder;

    static inline float outerShadowRadius = 5.0f;
    static inline float innerShadowRadius = 5.0f;
    static inline bool drawOuterShadow = true;
    static inline bool drawInnerShadow = true;
    static inline juce::Colour outerShadowColour = Colours::shadow;
    static inline juce::Colour innerShadowColour = Colours::shadow;
  };
  static inline struct OscillatorDisplay
  {
    static inline float margin = 20.0f;
    static inline float outerCornerSize = 10.0f;
    static inline float innerCornerSize = 9.0f;
    static inline float borderSize = 2.0f;
    static inline auto backgroundColour = Colours::foreground;
    static inline auto foregroundColour = Colours::background;
    static inline auto borderColour = juce::Colour(0, 0, 0);

    static inline float outerShadowRadius = 10.0f;
    static inline float innerShadowRadius = 100.0f;
    static inline bool drawOuterShadow = true;
    static inline bool drawInnerShadow = true;
    static inline juce::Colour outerShadowColour = Colours::shadow;
    static inline juce::Colour innerShadowColour = juce::Colour(255, 255, 255);
    ;
  };

  static inline int oscilloscopeBufferSize = 4096;
  static inline int oscilloscopeSamplesPerBlock = 1;
  static inline int oscilloscopeQuality = 1;
};
}