#pragma once

#include "dmt/version/Networking.h"
#include <JuceHeader.h>

namespace dmt {
namespace version {
class Manager : public juce::Thread
{
public:
  //============================================================================
  Manager()
    : juce::Thread("VersionManager")
  {
    startThread();
  }
  //============================================================================
  ~Manager() { stopThread(1000); }
  //============================================================================

protected:
  //============================================================================
  void run() override
  {
    while (!threadShouldExit()) {
      if (fetchLatestVersion()) {
        break;
      }
      wait(1000);
    }
  }
  //============================================================================
  bool fetchLatestVersion()
  {
    // TODO: Implement this method and return true if successful
    auto response = Networking::sendRequest("version?product=plasma");
    std::cout << response << std::endl;
    return false;
  }
  //============================================================================
private:
};
} // namespace version
} // namespace dmt