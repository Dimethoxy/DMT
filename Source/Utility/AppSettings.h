#pragma once

#include <JuceHeader.h>

namespace dmt {

struct AppSettings
{
  static inline juce::String name = "SuPerconductor";
  static inline struct Colours
  {
    static inline juce::Colour background900 = juce::Colour(13, 13, 13);
    static inline juce::Colour background800 = juce::Colour(26, 26, 26);
    static inline juce::Colour background700 = juce::Colour(38, 38, 38);
    static inline juce::Colour background600 = juce::Colour(51, 51, 51);
    static inline juce::Colour background500 = juce::Colour(64, 64, 64);
    static inline juce::Colour background400 = juce::Colour(77, 77, 77);
  };
  static inline struct Fonts
  {
    static inline juce::Typeface::Ptr regular =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsRegular_ttf,
        BinaryData::PoppinsRegular_ttfSize);

    static inline juce::Font regular = juce::Font(regular);

    static inline juce::Typeface::Ptr medium =
      juce::Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsMedium_ttf,
        BinaryData::PoppinsMedium_ttfSize);

    static inline juce::Font medium = juce::Font(medium);
  };
  static inline struct FontSizes
  {
    static inline int sliderInfoLabel = 16;
    static inline int sliderTitleLabel = 20;
    static inline int editorTitle = 48;
    static inline int triggerLabel = 30;
  };
  static inline int margin = 12;
  static inline float size = 0.0f;
  static inline float borderFactor = 5.0f;
  static inline int oscilloscopeBufferSize = 4096;
  static inline int oscilloscopeSamplesPerBlock = 1;
  static inline int oscilloscopeQuality = 1;
};
}