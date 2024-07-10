//==============================================================================
#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt
{
  namespace utility {
  struct Fonts
  {
    using Typeface = juce::Typeface;
    using Font = juce::Font;

    Typeface::Ptr getLightTypeface()
    {
      return Typeface::createSystemTypefaceFor(
        BinaryData::PoppinsLight_ttf, BinaryData::PoppinsLight_ttfSize);
    }
    Typeface::Ptr regularTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsRegular_ttf,
                                        BinaryData::PoppinsRegular_ttfSize);
    Typeface::Ptr mediumTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsMedium_ttf,
                                        BinaryData::PoppinsMedium_ttfSize);
    Typeface::Ptr boldTypeface =
      Typeface::createSystemTypefaceFor(BinaryData::PoppinsBold_ttf,
                                        BinaryData::PoppinsBold_ttfSize);

    Font light = Font(getLightTypeface()); // Font(FontOptions(lightTypeface));
    Font regular = Font(FontOptions(regularTypeface));
    Font medium = Font(FontOptions(mediumTypeface));
    Font bold = Font(FontOptions(boldTypeface));
  };
  } // namespace utility
} // namespace dmt
