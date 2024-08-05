#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <configuration/Options.h>
//==============================================================================
namespace dmt {
namespace configuration {
//==============================================================================
std::unique_ptr<juce::PropertiesFile> properties;
//==============================================================================
void
loadDefaultProperties(juce::String applicationName)
{
  auto options = getOptions(applicationName);
  if (properties == nullptr) {
    properties = std::make_unique<juce::PropertiesFile>(options);
  }
}
//==============================================================================
} // namespace configuration
} // namespace dmt