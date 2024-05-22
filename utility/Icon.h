#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace icons {
static inline std::unique_ptr<juce::Drawable>
getIcon(juce::String iconName)
{
  return juce::Drawable::createFromImageData(
    BinaryData::material_speed_svg, BinaryData::material_speed_svgSize);
};
// Returns the icon as an Image
} // namespace icons
} // namespace dmt