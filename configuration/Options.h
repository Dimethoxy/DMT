#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace configuration {
static inline auto
getOptions(juce::String applicationName)
{
  auto options = juce::PropertiesFile::Options();
  options.applicationName = applicationName;
  options.filenameSuffix = ".config";
  options.osxLibrarySubFolder = "Application Support";
  options.folderName = "Dimethoxy";
  options.storageFormat = juce::PropertiesFile::storeAsXML;
  options.commonToAllUsers = false;
  options.ignoreCaseOfKeyNames = true;
  options.doNotSave = false;
  options.millisecondsBeforeSaving = -1; // Only when explicitly calling
  return options;
}
} // namespace configuration
} // namespace dmt