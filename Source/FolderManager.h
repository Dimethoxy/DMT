/*
  ==============================================================================

    FolderManager.h
    Created: 28 Dec 2022 3:02:41pm
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
class FolderManager
{
public:
  inline static const juce::File& defaultDirectory =
    juce::File::getSpecialLocation(
      juce::File::SpecialLocationType::commonDocumentsDirectory)
      .getChildFile(ProjectInfo::companyName)
      .getChildFile(ProjectInfo::projectName);
};
}