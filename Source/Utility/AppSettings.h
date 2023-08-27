#pragma once

#include <JuceHeader.h>

namespace dmt {

struct Colours
{
  using Colour = juce::Colour;
  static inline Colour background = Colour(40, 42, 54);
  static inline Colour foreground = Colour(68, 71, 90);
  static inline Colour panelBorder = Colour(0, 0, 0);
  static inline Colour shadow = Colour(0, 0, 0);
  static inline Colour accent = Colour(80, 250, 123);

  static inline Colour accentAlpha = Colour::fromRGBA(80, 250, 123, 64);
  static inline Colour font = Colour(248, 248, 242);
};
struct AppSettings
{
  using Colour = juce::Colour;
  //==============================================================================
  static inline juce::String name = "Neutrino";
  static inline float size = 1.0f;
  //==============================================================================
  static inline struct LinearSlider
  {
    static inline float tumbSize = 1.3f;
  };
  //==============================================================================
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

    static inline Colour labelFontColour = Colours::font;
    static inline float sliderTitleLabelSize = 24.0f;
    static inline float sliderInfoLabelSize = 24.0f;
  };
  //==============================================================================
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
  static inline struct OscillatorTop
  {
    static inline auto backgroundColour = Colours::foreground;
    static inline auto fontColour = Colours::background;
    static inline auto drawFontOutline = false;
    static inline auto fontOutlineColour = Colours::accent;
    static inline auto fontOutlineThickness = 3.0f;

    static inline auto outerShadowRadius = 10.0f;
    static inline auto innerShadowRadius = 10.0f;
    static inline auto fontShadowRadius = 10.0f;
    static inline auto drawOuterShadow = true;
    static inline auto drawInnerShadow = false;
    static inline auto drawFontShadow = false;
    static inline auto outerShadowColour = Colour(0, 0, 0);
    static inline auto innerShadowColour = Colour(0, 0, 0);
    static inline auto fontShadowColour = Colours::accent;
  };
  static inline struct OscillatorDisplay
  {
    static inline auto margin = 20.0f;
    static inline auto outerCornerSize = 8.0f;
    static inline auto innerCornerSize = 7.0f;
    static inline auto borderSize = 2.0f;
    static inline auto backgroundColour = Colours::foreground;
    static inline auto foregroundColour = Colours::background;
    static inline auto borderColour = Colours::accent;

    static inline auto outerShadowRadius = 10.0f;
    static inline auto innerShadowRadius = 10.0f;
    static inline auto drawOuterShadow = false;
    static inline auto drawInnerShadow = true;
    static inline auto outerShadowColour = Colours::accent;
    static inline auto innerShadowColour = Colours::accent;

    static inline auto graphSize = 3.0f;
    static inline Colour graphColor = Colours::accent;
    static inline Colour integralColour = Colours::accentAlpha;
    ;
  };
  static inline struct ArcButton
  {
    static inline auto foregroundColour = Colours::background;
    static inline auto hoverColour = Colours::accentAlpha;
    static inline auto activeColour = Colours::accent;
    static inline auto borderSize = 2.0f;
    static inline auto borderColour = Colours::accent;

    static inline auto outerShadowRadius = 10.0f;
    static inline auto innerShadowRadius = 10.0f;
    static inline auto drawOuterShadow = false;
    static inline auto drawInnerShadow = true;
    static inline auto outerShadowColour = Colours::accent;
    static inline auto innerShadowColour = Colours::accent;
  };
  static inline struct Slider
  {};
  //==============================================================================
};
}
