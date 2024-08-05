#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <configuration/Options.h>
#include <configuration/Parameters.h>
//==============================================================================
namespace dmt {
namespace configuration {
//==============================================================================
class Properties
{
public:
  static void initialize(juce::String applicationName)
  {
    auto options = getOptions(applicationName);
    fileLock.enterWrite();
    if (file == nullptr) {
      file = new juce::PropertiesFile(options);
    }
    dmt::configuration::addParameters(file);
  }
  //==============================================================================
private:
  static juce::PropertiesFile* file;
  static juce::ReadWriteLock fileLock;
};
//==============================================================================
} // namespace configuration
} // namespace dmt