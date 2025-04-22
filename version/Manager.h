//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ███████ ██    ██   ███     ████
 * ██   ██ ██ ██  ██  ██ ██         ██    ██   ██ ██    ██  ██ ██     ██
 * ██████  ██ ██      ██ ███████    ██    ██   ██  ██████  ██   ██    ██
 *
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * This file is part of the Dimethoxy Library, a collection of essential
 * classes used across various Dimethoxy projects.
 * These files are primarily designed for internal use within our repositories.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * This class implements a thread that handles fetching the latest version and
 * comparing it with the current version.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

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
/**
 * @brief Manages the version checking and update notification process.
 *
 * @details
 * This class is responsible for fetching the latest version from the server,
 * comparing it with the current version, and determining if an update is
 * available.
 *
 * It runs in a background thread to avoid blocking the main application
 * thread. The thread will exit if the current version is the latest or if the
 * update notifications are disabled.
 *
 * @note This class is designed to be used as a single instance within the audio
 *       thread of the application.
 */
class Manager : public juce::Thread
{
  //==============================================================================
  // Alias for convenience
  using VersionArray = std::array<int, 3>;
  using Utility = dmt::version::Utility;

  //==============================================================================
  // Constants for thread management
  static constexpr int SERVER_WAIT_FOR_INITIALIZATION_TIMEOUT = 100;
  static constexpr int SERVER_RECONNECT_INTERVAL = 10000;
  static constexpr int THREAD_TIMEOUT = 1000;

public:
  //==============================================================================
  /**
   * @brief Constructs the Version Manager and starts the background thread.
   *
   * @details
   * Initializes the current version from ProjectInfo, and if update
   * notifications are enabled, starts the thread to check for updates.
   * Exits early if update notifications are disabled.
   */
  inline explicit Manager() noexcept
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

  //==============================================================================
  /**
   * @brief Destructor for Version Manager.
   *
   * @details
   * Ensures the background thread is stopped safely on destruction.
   */
  inline ~Manager() noexcept override
  {
    if (isThreadRunning()) {
      std::cout << "Aborting Version Manager thread..." << std::endl;
      stopThread(THREAD_TIMEOUT);
    }
  }

protected:
  //==============================================================================
  /**
   * @brief Main thread loop for version checking and update logic.
   *
   * @details
   * Waits for initialization, then repeatedly fetches the latest version,
   * compares it, fetches the download link, and determines if the thread
   * should exit. Designed for real-time safety and robust update logic.
   */
  inline void run() noexcept override
  {
    // Wait for the plugin to be initialized
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
  //==============================================================================
  /**
   * @brief Fetches the latest version from the server.
   *
   * @details
   * Sends a request to the server to fetch the latest version of the
   * application. Parses the response and stores the latest version in the Info
   * struct. Returns early if the thread should exit or if the latest version is
   * already fetched.
   */
  inline void fetchLatestVersion() noexcept
  {
    if (threadShouldExit())
      return;

    if (dmt::version::Info::latest != nullptr) {
      std::cout << "Latest version already fetched." << std::endl;
      return;
    }

    std::cout << "Fetching latest version..." << std::endl;
    const auto appName = dmt::Settings::appName.toLowerCase();
    const auto url = juce::String("version?product=" + appName);
    const auto response = Networking::sendRequest(url);

    if (response.isEmpty()) {
      std::cerr << "Failed to fetch latest version: Response was empty."
                << std::endl;
      return;
    }

    const auto versionString = Utility::parseResponseToVersionString(response);
    const auto versionArray = Utility::parseVersionStringToArray(versionString);

    dmt::version::Info::latest = std::make_unique<VersionArray>(versionArray);
  }

  //==============================================================================
  /**
   * @brief Compares the current version with the latest version.
   *
   * @details
   * Compares the current version with the latest version fetched from the
   * server. Updates the isLatest flag accordingly. Returns early if thread
   * should exit or if either version is missing.
   */
  inline void handleVersionComparison() noexcept
  {
    if (threadShouldExit())
      return;

    if (!dmt::version::Info::current || !dmt::version::Info::latest) {
      std::cerr << "Version comparison failed: one of the versions is null."
                << std::endl;
      return;
    }

    const int comparisonResult = Utility::compareVersions(
      *dmt::version::Info::current, *dmt::version::Info::latest);

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

  //==============================================================================
  /**
   * @brief Fetches the latest download link from the server.
   *
   * @details
   * Sends a request to the server to fetch the latest download link for the
   * application. Parses the response and stores the download link in the Info
   * struct. Returns early if the thread should exit or if the OS is unknown.
   */
  inline void fetchLatestDownloadLink() noexcept
  {
    if (threadShouldExit())
      return;

    std::cout << "Fetching latest download link..." << std::endl;

    const auto appName = dmt::Settings::appName.toLowerCase();

    String osName = "unknown";
    if (OS_IS_WINDOWS)
      osName = "windows";
    else if (OS_IS_DARWIN)
      osName = "mac";
    else if (OS_IS_LINUX)
      osName = "linux";

    if (osName == "unknown") {
      std::cerr << "Unknown Operating System. Cannot fetch download link."
                << std::endl;
      std::cout << "Version Manager exiting..." << std::endl;
      threadShouldExit();
      return;
    }

    const auto url =
      juce::String("download?product=" + appName + "&os=" + osName);
    const auto response = Networking::sendRequest(url);

    if (response.isEmpty()) {
      std::cerr << "Failed to fetch download link." << std::endl;
      return;
    }

    auto jsonResponse = juce::JSON::parse(response);

    if (!jsonResponse.isObject()) {
      std::cerr << "Failed to parse JSON response." << std::endl;
      return;
    }

    auto rawDownloadUrl = jsonResponse.getProperty("download_url", "");
    const auto downloadUrlString = rawDownloadUrl.toString();
    std::cout << "Download URL: " << downloadUrlString << std::endl;

    if (downloadUrlString.isEmpty()) {
      std::cerr << "Download URL is empty." << std::endl;
      return;
    }

    const juce::URL downloadUrl(downloadUrlString);
    if (!downloadUrl.isWellFormed()) {
      std::cerr << "Download URL is not well-formed." << std::endl;
      return;
    }

    std::cout << "Download URL is valid." << std::endl;
    dmt::version::Info::downloadLink =
      std::make_unique<String>(downloadUrlString);
  }

  //==============================================================================
  /**
   * @brief Handles the thread exit process.
   *
   * @details
   * Checks if the current version is the latest and if the download link is
   * available. If both conditions are met, requests the thread to exit.
   * Otherwise, does nothing. Called after each run of the thread.
   */
  inline void handleThreadExit() noexcept
  {
    if (threadShouldExit())
      return;

    if (!dmt::version::Info::isLatest)
      return;

    if (*dmt::version::Info::isLatest) {
      std::cout << "Version Manager exiting..." << std::endl;
      signalThreadShouldExit();
      return;
    }

    if (dmt::version::Info::downloadLink == nullptr) {
      return;
    }

    std::cout << "Version Manager exiting..." << std::endl;
    signalThreadShouldExit();
  }

  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Manager)
};

} // namespace version
} // namespace dmt