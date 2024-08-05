#pragma once

#include "utility/Settings.h"
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
  options.storageFormat = juce::PropertiesFile::storeAsXML;
  options.commonToAllUsers = false;
  options.ignoreCaseOfKeyNames = true;
  options.doNotSave = false;
  options.millisecondsBeforeSaving = -1; // Only when explicitly calling

  if (OS_IS_WINDOWS) {
    options.folderName = "Dimethoxy/" + applicationName;
  } else if (OS_IS_DARWIN) {
    options.folderName = "Dimethoxy/" + applicationName;
  } else if (OS_IS_LINUX) {
    options.folderName = ".config/Dimethoxy/" + applicationName;
  }
  
  return options;
}
} // namespace configuration
} // namespace dmt