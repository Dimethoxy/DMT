#pragma once
//==============================================================================
#include "dmt/utility/Settings.h"
#include "dmt/version/Info.h"
#include "dmt/version/Networking.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace version {
//==============================================================================
constexpr auto SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT = 100;
constexpr auto SERVER_RECONNECT_INTERVAL = 10000;
constexpr auto THREAD_TIMEOUT = 1000;
//==============================================================================
class Manager : public juce::Thread
{
  using VersionArray = std::array<int, 3>;

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
    dmt::version::Info::current = std::make_unique<VersionArray>(versionArray);

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
    // Wait for the plugin to be initialized
    wait(SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT);

    // Continuously try to fetch the latest version
    while (!threadShouldExit()) {
      if (!dmt::Settings::appName.isEmpty()) {
        if (fetchLatestVersion()) {
          handleVersionComparison();
          break; // Exit the loop if fetching the version is successful
        }
      }
      // Wait before retrying if fetching the version fails
      wait(SERVER_RECONNECT_INTERVAL);
    }
  }

private:
  void handleVersionComparison()
  {
    // Make sure both versions are available or if they are nullptr
    if (!dmt::version::Info::current || !dmt::version::Info::latest) {
      std::cerr << "Version comparison failed: one of the versions is null."
                << std::endl;
      return;
    }

    // Compare the current version with the latest version
    const int comparisonResult = compareVersions(*dmt::version::Info::current,
                                                 *dmt::version::Info::latest);

    // Process the result of the comparison
    switch (comparisonResult) {
      case 0:
        std::cout << "You are using the latest version." << std::endl;
        dmt::version::Info::isLatest = std::make_unique<bool>(true);
        break;
      case 1:
        std::cout << "Newer version available." << std::endl;
        dmt::version::Info::isLatest = std::make_unique<bool>(false);
        break;
      case 2:
        std::cout << "This is a future version." << std::endl;
        dmt::version::Info::isLatest = std::make_unique<bool>(true);
        break;
      default:
        std::cerr << "Unexpected version comparison result." << std::endl;
        break;
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
    const auto versionArray = parseVersionStringToArray(versionString);
    dmt::version::Info::latest = std::make_unique<VersionArray>(versionArray);

    // Let's also find the download link
    const auto donwload = fetchLatestDownloadLink();

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
  VersionArray parseVersionStringToArray(const juce::String& versionString)
  {
    auto versionArray = juce::StringArray::fromTokens(versionString, ".", "");
    VersionArray result;
    for (int i = 0; i < 3; i++) {
      result[static_cast<std::size_t>(i)] = versionArray[i].getIntValue();
    }
    std::cout << "- Major:" << result[0] << std::endl;
    std::cout << "- Minor:" << result[1] << std::endl;
    std::cout << "- Patch:" << result[2] << std::endl;
    return result;
  }
  //===========================================================================
  int inline compareVersions(const VersionArray& version1,
                             const VersionArray& version2)
  {
    for (std::size_t i = 0; i < version1.size(); ++i) {
      if (version1[i] < version2[i])
        return 2; // version2 is greater than version1
      else if (version1[i] > version2[i])
        return 1; // version1 is greater than version2
    }
    return 0; // Both versions are equal
  }
  //============================================================================
  juce::String fetchLatestDownloadLink()
  {
    // Debugging
    std::cout << "Fetching latest download link..." << std::endl;

    // Find app name
    const auto appName = dmt::Settings::appName.toLowerCase();

    // Determine the OS
    String osName = "unknown";
    if (OS_IS_WINDOWS)
      osName = "windows";
    else if (OS_IS_DARWIN)
      osName = "macos";
    else if (OS_IS_LINUX)
      osName = "linux";

    // If the OS is still unknown, return an empty string
    if (osName == "unknown") {
      std::cerr << "Unknown Operating System. Cannot fetch download link."
                << std::endl;
      return {};
    }

    const auto url =
      juce::String("download?product=" + appName + "&os=" + osName);
    const auto response = Networking::sendRequest(url);
    if (response.isEmpty()) {
      std::cerr << "Failed to fetch download link." << std::endl;
      return {};
    }
    // std::cout << "Response: " << response << std::endl;

    // Let's parse the response as JSON
    auto jsonResponse = juce::JSON::parse(response);

    // Let's parse the JSON response
    if (jsonResponse.isObject()) {
      auto rawDownloadUrl = jsonResponse.getProperty("download_url", "");
      const auto downloadUrlString = rawDownloadUrl.toString();
      std::cout << "Download URL: " << downloadUrlString << std::endl;
      if (!downloadUrlString.isEmpty()) {
        const juce::URL downloadUrl(downloadUrlString);
        // Check if the download URL is valid
        if (downloadUrl.isWellFormed()) {
          std::cout << "Download URL is valid." << std::endl;
          return downloadUrlString; // Successfully fetched download URL
        } else {
          std::cerr << "Download URL is not valid." << std::endl;
        }
      }
    }

    // If we landed here, it means we couldn't find the download URL
    std::cerr << "Couldn't find download URL in the response." << std::endl;
    return {};
  }
};
} // namespace version
} // namespace dmt