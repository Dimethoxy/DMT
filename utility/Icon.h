#pragma once
//==============================================================================
#include "BinaryData.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace icons {
//==============================================================================
static inline std::unique_ptr<juce::Drawable>
getIcon(const juce::String iconName)
{
  if (iconName == "OscilloscopeZoom")
    return juce::Drawable::createFromImageData(BinaryData::speed_svg,
                                               BinaryData::speed_svgSize);
  if (iconName == "OscilloscopeThickness")
    return juce::Drawable::createFromImageData(BinaryData::thickness_svg,
                                               BinaryData::thickness_svgSize);
  if (iconName == "OscilloscopeGain")
    return juce::Drawable::createFromImageData(BinaryData::height_svg,
                                               BinaryData::height_svgSize);
  return nullptr;
};
//==============================================================================
static inline float
getPadding(const juce::String iconName)
{
  if (iconName == "OscilloscopeZoom")
    return 3.0f;
  if (iconName == "OscilloscopeThickness")
    return 5.0f;
  if (iconName == "OscilloscopeGain")
    return 5.0f;
  return 0.0f;
};
//==============================================================================
} // namespace icons
} // namespace dmt