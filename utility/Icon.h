#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace icons {
enum class Icon
{
  Speed,
  Height,
  Thickness,
};
const auto
getIcon(Icon icon)
{
  switch (icon) {
    case Icon::Speed:

  }      return juce::Drawable::createFromImageData(BinaryData::speed_svg,
                                                 BinaryData::speed_svgSize);
};
// Returns the icon as an Image
} // namespace icons
} // namespace dmt