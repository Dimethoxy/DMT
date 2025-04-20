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
    // Debugging
    std::cout << "Version Manager Initialized" << std::endl;
    std::cout << "Parsing current version..." << std::endl;

    // Parse the current version into settings
    const auto versionString = ProjectInfo::versionString;
    std::cout << "Version String: " << versionString << std::endl;

    // Parse the version string into an array
    const auto versionArray = parseVersionStringToArray(versionString);

    // Start the thread to fetch the latest version
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
    std::cout << "Fetching latest version..." << std::endl;
    const auto appName = dmt::Settings::appName.toLowerCase();
    const auto url = juce::String("version?product=" + appName);
    const auto response = Networking::sendRequest(url);
    if (response.isEmpty()) {
      return false;
    }
    const auto versionString = parseResponseToVersionString(response);
    dmt::Settings::latestVersion = parseVersionStringToArray(versionString);
    return true;
  }
  //============================================================================
  juce::String parseResponseToVersionString(const juce::String& response)
  {
    int start = response.indexOf("version") + 8;
    auto secondHalf = response.substring(start);
    auto tokens = juce::StringArray::fromTokens(secondHalf, "\"", "");
    auto versionString = tokens[1];
    std::cout << "Version String: " << versionString << std::endl;
    return versionString;
  }
  //============================================================================
  std::array<int, 3> parseVersionStringToArray(
    const juce::String& versionString)
  {
    auto versionArray = juce::StringArray::fromTokens(versionString, ".", "");
    std::array<int, 3> result;
    for (int i = 0; i < 3; i++) {
      result[static_cast<std::size_t>(i)] = versionArray[i].getIntValue();
    }
    std::cout << "- Major:" << result[0] << std::endl;
    std::cout << "- Minor:" << result[1] << std::endl;
    std::cout << "- Patch:" << result[2] << std::endl;
    return result;
  }
  //===========================================================================
  int inline compareVersions(const std::array<int, 3>& version1,
                             const std::array<int, 3>& version2)
  {
    for (std::size_t i = 0; i < version1.size(); ++i) {
      if (version1[i] < version2[i])
        return 2; // version2 is greater than version1
      else if (version1[i] > version2[i])
        return 1; // version1 is greater than version2
    }
    return 0; // Both versions are equal
  }
};
} // namespace version
} // namespace dmt