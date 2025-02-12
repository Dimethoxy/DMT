//==============================================================================
#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace utility {
struct Fonts
{
  using Typeface = juce::Typeface;
  using Font = juce::Font;

  Typeface::Ptr displayTypeface = Typeface::createSystemTypefaceFor(
    BinaryData::SedgwickAveDisplayRegular_ttf,
    BinaryData::SedgwickAveDisplayRegular_ttfSize);

  Typeface::Ptr lightTypeface =
    Typeface::createSystemTypefaceFor(BinaryData::PoppinsLight_ttf,
                                      BinaryData::PoppinsLight_ttfSize);
  Typeface::Ptr regularTypeface =
    Typeface::createSystemTypefaceFor(BinaryData::PoppinsRegular_ttf,
                                      BinaryData::PoppinsRegular_ttfSize);
  Typeface::Ptr mediumTypeface =
    Typeface::createSystemTypefaceFor(BinaryData::PoppinsMedium_ttf,
                                      BinaryData::PoppinsMedium_ttfSize);
  Typeface::Ptr boldTypeface =
    Typeface::createSystemTypefaceFor(BinaryData::PoppinsBold_ttf,
                                      BinaryData::PoppinsBold_ttfSize);

  Font display = Font(FontOptions(displayTypeface));
  Font light = Font(FontOptions(lightTypeface));
  Font regular = Font(FontOptions(regularTypeface));
  Font medium = Font(FontOptions(mediumTypeface));
  Font bold = Font(FontOptions(boldTypeface));
};
} // namespace utility
} // namespace dmt
