//============================================================================

#pragma once

//==============================================================================

#include "dmt/utility/Settings.h"
#include "dmt/version/Info.h"
#include "dmt/version/Networking.h"
#include "dmt/version/Utility.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace version {

//==============================================================================

constexpr auto SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT = 100;
constexpr auto SERVER_RECONNECT_INTERVAL = 10000;
constexpr auto THREAD_STOP_TIMEOUT = 5000; // We are not in a rush to exit
constexpr auto THREAD_ABORT_TIMEOUT = 1000;

//==============================================================================

class Manager : public juce::Thread
{
  using VersionArray = std::array<int, 3>;
  using Utility = dmt::version::Utility;

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
    const auto versionArray = Utility::parseVersionStringToArray(versionString);
    dmt::version::Info::current = std::make_unique<VersionArray>(versionArray);

    // Exit early if update notifications are disabled
    if (DMT_DISABLE_UPDATE_NOTIFICATION) {
      std::cout << "Update notifications are disabled." << std::endl;
      std::cout << "Version Manager exiting early..." << std::endl;
      return;
    }

    // Start the thread to fetch the latest version
    startThread();
  }
  //============================================================================
  ~Manager()
  {
    if (isThreadRunning()) {
      std::cout << "Aborting Version Manager thread..." << std::endl;
      stopThread(THREAD_ABORT_TIMEOUT);
    }
  }

protected:
  //============================================================================
  void run() override
  { // Wait for the plugin to be initialized
    wait(SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT);

    // Continuously try to fetch the latest version
    while (!threadShouldExit()) {
      if (!dmt::Settings::appName.isEmpty()) {
        fetchLatestVersion();
        handleVersionComparison();
        fetchLatestDownloadLink();
        handleThreadExit();
      }
      // Wait before retrying if fetching the version fails
      wait(SERVER_RECONNECT_INTERVAL);
    }
  }

private:
  //============================================================================
  /**
   * @brief Fetches the latest version from the server.
   *
   * @details This function sends a request to the server to fetch the latest
   *          version of the application. It parses the response and stores
   * the latest version in the Info struct.
   */
  void fetchLatestVersion() noexcept
  {
    // If the latest version is already fetched, return early
    if (dmt::version::Info::latest != nullptr) {
      std::cout << "Latest version already fetched." << std::endl;
      return;
    }

    // Attempt to fetch the latest version from the server
    std::cout << "Fetching latest version..." << std::endl;
    const auto appName = dmt::Settings::appName.toLowerCase();
    const auto url = juce::String("version?product=" + appName);
    const auto response = Networking::sendRequest(url);

    // Check if the response is empty
    if (response.isEmpty()) {
      std::cerr << "Failed to fetch latest version: Response was empty."
                << std::endl;
      return;
    }

    // Parse the response to extract the version string
    const auto versionString = Utility::parseResponseToVersionString(response);
    const auto versionArray = Utility::parseVersionStringToArray(versionString);

    // Store the latest version in the Info struct
    dmt::version::Info::latest = std::make_unique<VersionArray>(versionArray);
  }
  //============================================================================
  /**
   * @brief Compares the current version with the latest version.
   *
   * @details This function compares the current version with the latest
   *          version fetched from the server. It updates the isLatest flag
   *          accordingly.
   */
  void handleVersionComparison() noexcept
  {
    // Make sure both versions are available
    if (!dmt::version::Info::current || !dmt::version::Info::latest) {
      std::cerr << "Version comparison failed: one of the versions is null."
                << std::endl;
      return;
    }

    // Compare the current version with the latest version
    const int comparisonResult = Utility::compareVersions(
      *dmt::version::Info::current, *dmt::version::Info::latest);

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
  /**
   * @brief Fetches the latest download link from the server.
   *
   * @details This function sends a request to the server to fetch the latest
   *          download link for the application. It parses the response and
   *          stores the download link in the Info struct.
   */
  void fetchLatestDownloadLink() noexcept
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
      osName = "mac";
    else if (OS_IS_LINUX)
      osName = "linux";

    // If the OS is still unknown, return an empty string
    if (osName == "unknown") {
      std::cerr << "Unknown Operating System. Cannot fetch download link."
                << std::endl;
      // If we land here, continueing makes no sense and we should stop
      stopThread(THREAD_STOP_TIMEOUT);
      std::cout << "Version Manager exiting..." << std::endl;
      return;
    }

    // Attempt to fetch the download link from the server
    const auto url =
      juce::String("download?product=" + appName + "&os=" + osName);
    const auto response = Networking::sendRequest(url);

    // Check if the response is empty
    if (response.isEmpty()) {
      std::cerr << "Failed to fetch download link." << std::endl;
      return;
    }

    // Let's parse the response as JSON
    auto jsonResponse = juce::JSON::parse(response);

    // Let's parse the JSON response
    if (!jsonResponse.isObject()) {
      std::cerr << "Failed to parse JSON response." << std::endl;
      return;
    }

    // Check if the JSON response contains the "download_url" property
    auto rawDownloadUrl = jsonResponse.getProperty("download_url", "");
    const auto downloadUrlString = rawDownloadUrl.toString();
    std::cout << "Download URL: " << downloadUrlString << std::endl;

    // Check if the download URL is empty
    if (downloadUrlString.isEmpty()) {
      std::cerr << "Download URL is empty." << std::endl;
      return;
    }

    // Check if the download URL is valid
    const juce::URL downloadUrl(downloadUrlString);
    if (!downloadUrl.isWellFormed()) {
      std::cerr << "Download URL is not well-formed." << std::endl;
      return;
    }

    // Successfully parsed the download URL
    std::cout << "Download URL is valid." << std::endl;
    dmt::version::Info::downloadLink =
      std::make_unique<String>(downloadUrlString);
  }
  //============================================================================
  /**
   * @brief Handles the thread exit process.
   *
   * @details This function checks if the current version is the latest
   *          and if the download link is available. If both conditions are
   *          met, it stops the thread and exits. Otherwise, it does nothing.
   *          This function is called after each run of the thread.
   */
  void handleThreadExit() noexcept
  {
    // If we don't know if we are on lastest version return early
    if (!dmt::version::Info::isLatest)
      return; // Can't stop the tread yet

    // So we know if we are on the latest version, if we are we can exit
    if (*dmt::version::Info::isLatest) {
      // On latest version we don't even need to check the download url
      stopThread(THREAD_STOP_TIMEOUT); // So exit the thread
      std::cout << "Version Manager exiting..." << std::endl;
      return;
    }

    // So we are not on the latest version, let's check the download url
    if (dmt::version::Info::downloadLink == nullptr) {
      // Something went wrong, we don't have a download link.
      return; // So we exit without stopping the thread.
    }

    // Download link is fine, so we can exit the thread
    stopThread(THREAD_STOP_TIMEOUT);
    std::cout << "Version Manager exiting..." << std::endl;
  }
};
} // namespace version
} // namespace dmt