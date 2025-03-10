#pragma once
//==============================================================================
#include "dmt/version/Networking.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
constexpr auto SERVER_RECONNECT_INTERVAL = 10000;
//==============================================================================
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
      wait(SERVER_RECONNECT_INTERVAL);
    }
  }
  //============================================================================
  bool fetchLatestVersion()
  {
    // TODO: Implement this method and return true if successful
    auto response = Networking::sendRequest("version?product=plasma");
    latestVersion = response;
    return false;
  }
  //============================================================================
private:
  juce::String latestVersion;
};
} // namespace version
} // namespace dmt