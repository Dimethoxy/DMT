#pragma once

#include <JuceHeader.h>

namespace dmt {

struct AppSettings
{
  using Colour = juce::Colour;

  static inline juce::String name = "Neutrino";
  static inline struct Colours
  {
    static inline Colour background = Colour(18, 18, 18);
    static inline Colour foreground = Colour(29, 29, 29);
    static inline Colour panelBorder = Colour(0, 0, 0);
    static inline Colour shadow = Colour(0, 0, 0);
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
    static inline auto margin = 20.0f;
    static inline auto outerCornerSize = 10.0f;
    static inline auto innerCornerSize = 9.0f;
    static inline auto borderSize = 2.0f;
    static inline auto backgroundColour = Colours::background;
    static inline auto foregroundColour = Colours::foreground;
    static inline auto borderColour = Colour(0, 0, 0);

    static inline auto outerShadowRadius = 5.0f;
    static inline auto innerShadowRadius = 5.0f;
    static inline auto drawOuterShadow = true;
    static inline auto drawInnerShadow = false;
    static inline auto outerShadowColour = Colour(0, 0, 0);
    static inline auto innerShadowColour = Colour(0, 0, 0);
  };
  static inline struct OscillatorDisplay
  {
    static inline auto margin = 20.0f;
    static inline auto outerCornerSize = 8.0f;
    static inline auto innerCornerSize = 7.0f;
    static inline auto borderSize = 2.0f;
    static inline auto backgroundColour = Colours::foreground;
    static inline auto foregroundColour = Colours::background;
    static inline auto borderColour = Colour(158, 85, 252);

    static inline auto outerShadowRadius = 10.0f;
    static inline auto innerShadowRadius = 50.0f;
    static inline auto drawOuterShadow = true;
    static inline auto drawInnerShadow = true;
    static inline auto outerShadowColour = Colour(0, 0, 0);
    static inline auto innerShadowColour = Colour(158, 85, 252);

    static inline auto graphSize = 3.0f;
    static inline Colour graphColor = Colour(158, 85, 252);
    static inline Colour integralColour = Colour::fromRGBA(158, 85, 252, 64);
    ;
  };

  static inline int oscilloscopeBufferSize = 4096;
  static inline int oscilloscopeSamplesPerBlock = 1;
  static inline int oscilloscopeQuality = 1;
};
}