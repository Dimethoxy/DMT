#pragma once

#include <JuceHeader.h>
#include <configuration/Options.h>

namespace dmt {
namespace configuration {
static inline juce::PropertiesFile
loadDefaultProperties(juce::String applicationName)
{
  auto options = getOptions(applicationName);
  auto properties = juce::PropertiesFile(options);
  // return properties;
}
} // namespace configuration
} // namespace dmt