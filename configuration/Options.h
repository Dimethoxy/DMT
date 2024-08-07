#pragma once

#include "utility/Settings.h"
#include <JuceHeader.h>

namespace dmt {
namespace configuration {
static inline auto
getOptions()
{
  auto options = juce::PropertiesFile::Options();
  options.applicationName = ProjectInfo::projectName;
  options.filenameSuffix = ".config";
  options.osxLibrarySubFolder = "Application Support";
  options.storageFormat = juce::PropertiesFile::storeAsXML;
  options.commonToAllUsers = false;
  options.ignoreCaseOfKeyNames = true;
  options.doNotSave = false;
  options.millisecondsBeforeSaving = -1; // Only when explicitly calling

  if (OS_IS_WINDOWS) {
    options.folderName = "Dimethoxy/" + ProjectInfo::projectName;
  } else if (OS_IS_DARWIN) {
    options.folderName = "Dimethoxy/" + ProjectInfo::projectName;
  } else if (OS_IS_LINUX) {
    options.folderName = ".config/Dimethoxy/" + ProjectInfo::projectName;
  }

  return options;
}
} // namespace configuration
} // namespace dmt