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
  if (iconName == "Settings")
    return juce::Drawable::createFromImageData(BinaryData::gear_svg,
                                               BinaryData::gear_svgSize);
  if (iconName == "Back")
    return juce::Drawable::createFromImageData(BinaryData::back_svg,
                                               BinaryData::back_svgSize);
  if (iconName == "HideHeader")
    return juce::Drawable::createFromImageData(BinaryData::angles_up_svg,
                                               BinaryData::angles_up_svgSize);
  if (iconName == "Bypass")
    return juce::Drawable::createFromImageData(BinaryData::bypass_svg,
                                               BinaryData::bypass_svgSize);
  if (iconName == "Download")
    return juce::Drawable::createFromImageData(BinaryData::download_svg,
                                               BinaryData::download_svgSize);
  if (iconName == "Presets")
    return juce::Drawable::createFromImageData(BinaryData::presets_svg,
                                               BinaryData::presets_svgSize);
  if (iconName == "Close")
    return juce::Drawable::createFromImageData(BinaryData::close_svg,
                                               BinaryData::close_svgSize);
  if (iconName == "Save")
    return juce::Drawable::createFromImageData(BinaryData::save_svg,
                                               BinaryData::save_svgSize);
  if (iconName == "Reload")
    return juce::Drawable::createFromImageData(BinaryData::reload_svg,
                                               BinaryData::reload_svgSize);
  if (iconName == "Info")
    return juce::Drawable::createFromImageData(BinaryData::info_svg,
                                               BinaryData::info_svgSize);
  if (iconName == "Warning")
    return juce::Drawable::createFromImageData(BinaryData::warning_svg,
                                               BinaryData::warning_svgSize);
  if (iconName == "Error")
    return juce::Drawable::createFromImageData(BinaryData::error_svg,
                                               BinaryData::error_svgSize);
  if (iconName == "Success")
    return juce::Drawable::createFromImageData(BinaryData::success_svg,
                                               BinaryData::success_svgSize);
  return nullptr;
};
//==============================================================================
static inline float
getPadding(const juce::String iconName)
{
  if (iconName == "OscilloscopeZoom")
    return 4.0f;
  if (iconName == "OscilloscopeThickness")
    return 5.0f;
  if (iconName == "OscilloscopeGain")
    return 3.5f;
  if (iconName == "Settings")
    return 5.0f;
  if (iconName == "Back")
    return 5.0f;
  if (iconName == "HideHeader")
    return 5.0f;
  if (iconName == "Bypass")
    return 5.0f;
  if (iconName == "Download")
    return 5.0f;
  if (iconName == "Presets")
    return 5.0f;
  if (iconName == "Close")
    return 0.0f;
  if (iconName == "Save")
    return 5.0f;
  if (iconName == "Reload")
    return 4.0f;
  if (iconName == "Info")
    return 4.0f;
  if (iconName == "Warning")
    return 4.0f;
  if (iconName == "Error")
    return 4.0f;
  if (iconName == "Success")
    return 4.0f;
  return 0.0f;
};
//==============================================================================
} // namespace icons
} // namespace dmt