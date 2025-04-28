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
 * Singleton class to manage application properties with optimized
 * performance.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>
#include <configuration/Options.h>

//==============================================================================

namespace dmt {
namespace configuration {

//==============================================================================
/**
 * @class Properties
 * @brief Class to manage application properties
 */
class Properties
{
public:
  //============================================================================
  /**
   * @brief Initialize the properties with options and settings.
   */
  void initialize() noexcept
  {
    auto options = dmt::configuration::getOptions();
    file.setStorageParameters(options);
    auto settings = file.getUserSettings();

    // Set the fallback property set by extracting it from the container
    fallbackPropertySet = dmt::Settings::container.toPropertySet();
    settings->setFallbackPropertySet(&fallbackPropertySet);

    // Add missing keys from the fallback property set
    bool newKeysAdded = false;
    const auto& fallbackKeys = fallbackPropertySet.getAllProperties();
    for (const auto& key : fallbackKeys.getAllKeys()) {
      if (!settings->containsKey(key)) {
        settings->setValue(key, fallbackKeys[key]);
        newKeysAdded = true; // Mark that new keys were added
      }
    }

    // Remove the "initialized" flag if new keys were added
    if (newKeysAdded && settings->containsKey("initialized")) {
      settings->removeValue("initialized");
    }

    // Mark as initialized and save the settings
    if (!settings->containsKey("initialized")) {
      settings->setValue("initialized", true);
    }
    settings->saveIfNeeded();

    // Now we apply the settings to the container
    dmt::Settings::container.applyPropertySet(settings);

    // Set the app name
    auto appName = options.applicationName;
    dmt::Settings::appName = appName;
  }

  /**
   * @brief Save the current container settings to the file system.
   */
  void saveCurrentSettings()
  {
    auto* settings = file.getUserSettings();
    auto currentSet = dmt::Settings::container.toPropertySet();
    for (const auto& key : currentSet.getAllProperties().getAllKeys()) {
      settings->setValue(key, currentSet.getValue(key));
    }
    settings->saveIfNeeded();
  }

  /**
   * @brief Reset the container and file to fallback (default) values.
   */
  void resetToFallback()
  {
    auto* settings = file.getUserSettings();
    // Overwrite container with fallback
    dmt::Settings::container.applyPropertySet(&fallbackPropertySet);
    // Overwrite file with fallback
    for (const auto& key :
         fallbackPropertySet.getAllProperties().getAllKeys()) {
      settings->setValue(key, fallbackPropertySet.getValue(key));
    }
    settings->saveIfNeeded();
  }

private:
  juce::ApplicationProperties file;
  juce::PropertySet fallbackPropertySet;
};
} // namespace configuration
} // namespace dmt