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
      file = new juce::ApplicationProperties();
      file->setStorageParameters(options);
    }
    dmt::configuration::addParameters(file->getUserSettings());
  }
  //==============================================================================
private:
  static juce::ApplicationProperties* file;
  static juce::ReadWriteLock fileLock;
};
//==============================================================================
} // namespace configuration
} // namespace dmt