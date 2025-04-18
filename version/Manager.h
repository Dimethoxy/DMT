#pragma once
//==============================================================================
#include "dmt/utility/Settings.h"
#include "dmt/version/Networking.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
constexpr auto SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT = 100;
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
    // Wait for the the plugin to be initialized
    wait(SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT);

    // Continuously try to fetch the latest version
    while (!threadShouldExit()) {
      // Check if the app name is initialized before proceeding
      const auto isInitialized = !dmt::Settings::appName.isEmpty();
      if (isInitialized) {
        if (fetchLatestVersion())
          break; // Exit the loop if the version is fetched successfully
      }
      // If we couldn't fetch the version, wait for a while before trying again
      wait(SERVER_RECONNECT_INTERVAL);
    }
  }
  //============================================================================
  bool fetchLatestVersion()
  {
    std::cout << "Fetching latest version" << std::endl;
    const auto appName = dmt::Settings::appName.toLowerCase();
    const auto url = juce::String("version?product=" + appName);
    const auto response = Networking::sendRequest(url);
    if (response.isEmpty()) {
      return false;
    }
    dmt::Settings::latestVersion = parseVersion(response);
    return true;
  }
  //============================================================================
  std::array<int, 3> parseVersion(const juce::String& version)
  {

    int start = version.indexOf("version") + 8;
    auto secondHalf = version.substring(start);
    auto tokens = juce::StringArray::fromTokens(secondHalf, "\"", "");
    auto versionString = tokens[1];
    auto versionArray = juce::StringArray::fromTokens(versionString, ".", "");
    std::array<int, 3> result;
    for (int i = 0; i < 3; i++) {
      result[i] = versionArray[i].getIntValue();
    }
    std::cout << "Parsed Version: " << versionString << std::endl;
    std::cout << "- Major:" << result[0] << std::endl;
    std::cout << "- Minor:" << result[1] << std::endl;
    std::cout << "- Patch:" << result[2] << std::endl;
    return result;
  }
  //============================================================================
};
} // namespace version
} // namespace dmt