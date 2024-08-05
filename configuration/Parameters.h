#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <configuration/Options.h>
//==============================================================================
namespace dmt {
namespace configuration {
//==============================================================================
void
addParameters(juce::PropertiesFile* file)
{
  file->setValue("Framerate", 30);
  file->setValue("UnixFontScalingFactor", 0.9);
  file->setValue("ShowDebugGrid", false);
}
//==============================================================================
} // namespace configuration
} // namespace dmt