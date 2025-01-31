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
#include <configuration/Parameters.h>

//==============================================================================

namespace dmt {
namespace configuration {

//==============================================================================
/**
 * @class Properties
 * @brief Singleton class to manage application properties with optimized
 * performance.
 */
class Properties
{
public:
  Properties() = delete;
  Properties(const Properties& _obj) = delete;
  ~Properties() noexcept
  {
    file.saveIfNeeded();
    file.closeFiles();
  }

  //============================================================================
  /**
   * @brief Get the singleton instance of Properties.
   * @return Shared pointer to the Properties instance.
   */
  [[nodiscard]] static std::shared_ptr<Properties> getInstance() noexcept
  {
    if (instance == nullptr) {
      instance = std::make_shared<Properties>();
    }
    return instance;
  }

  //============================================================================
  /**
   * @brief Initialize the properties with options and settings.
   */
  void initialize() noexcept
  {
    auto options = dmt::configuration::getOptions();
    file.setStorageParameters(options);

    auto settings = file.getUserSettings();
    fallbackPropertySet = dmt::configuration::getPropertySet();
    settings->setFallbackPropertySet(&fallbackPropertySet);
  }

private:
  static inline std::shared_ptr<Properties> instance = nullptr;
  juce::ApplicationProperties file;
  juce::PropertySet fallbackPropertySet;
};

} // namespace configuration
} // namespace dmt