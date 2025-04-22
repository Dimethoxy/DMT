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
 * Type-safe settings container for Dimethoxy Audio applications.
 * Provides runtime-checked, variant-based storage for configuration values.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace configuration {

//==============================================================================
/**
 * @brief A type-safe container for application settings.
 *
 * @details
 * This class allows adding and retrieving settings with different types.
 * It ensures that the types of the settings are consistent, and provides
 * type-safe access to the values. All access is checked at runtime.
 */
class Container
{
  //==============================================================================
  /**
   * @brief Variant type for storing heterogeneous settings.
   *
   * @details
   * The possible types are: juce::String, juce::Colour, int, float, and bool.
   * Used for type-safe, runtime-checked configuration storage.
   */
  using SettingValue =
    std::variant<juce::String, juce::Colour, int, float, bool>;

public:
  //==============================================================================
  /**
   * @brief Retrieves a setting by its name with type safety.
   *
   * @tparam T The type of the setting to retrieve.
   * @param _name The name of the setting.
   * @return A mutable reference to the setting value of type T.
   *
   * @throws std::runtime_error If the setting is not found or the type does not
   * match.
   *
   * @details
   * Throws if the type does not match or the setting does not exist.
   * Use for real-time safe, type-checked access to configuration.
   */
  template<typename T>
  inline T& get(const juce::String _name)
  {
    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (std::holds_alternative<T>(it->second)) {
        return std::get<T>(it->second);
      } else {
        jassertfalse;
        throw std::runtime_error("Type mismatch for setting: " +
                                 _name.toStdString());
      }
    } else {
      jassertfalse;
      throw std::runtime_error("Setting not found: " + _name.toStdString());
    }
  }

  //==============================================================================
  /**
   * @brief Adds a new setting or ensures consistency for an existing one.
   *
   * @tparam T The type of the setting to add.
   * @param _name The name of the setting.
   * @param _value The value to associate with the setting.
   * @return A mutable reference to the setting value of type T.
   *
   * @throws std::runtime_error If there is a type mismatch when adding an
   * existing setting.
   *
   * @details
   * If the setting already exists, the type must match the stored type.
   * If it doesn't match, an error is thrown. If the setting doesn't exist,
   * it is added to the collection.
   */
  template<typename T>
  inline T& add(const juce::String _name, const T _value)
  {
    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (!std::holds_alternative<T>(it->second)) {
        jassertfalse;
        throw std::runtime_error("Type mismatch for setting: " +
                                 _name.toStdString());
      }
    } else {
      settings[_name] = _value;
    }
    return std::get<T>(settings[_name]);
  }

  //==============================================================================
  /**
   * @brief Converts all settings to a juce::PropertySet.
   *
   * @return A juce::PropertySet containing all settings as string values.
   *
   * @details
   * Used for serialization or export of settings to JUCE property sets.
   */
  inline juce::PropertySet toPropertySet() const
  {
    juce::PropertySet propertySet;
    for (const auto& [key, value] : settings) {
      if (std::holds_alternative<juce::String>(value)) {
        propertySet.setValue(key, std::get<juce::String>(value));
      } else if (std::holds_alternative<juce::Colour>(value)) {
        propertySet.setValue(key, std::get<juce::Colour>(value).toString());
      } else if (std::holds_alternative<int>(value)) {
        propertySet.setValue(key, std::get<int>(value));
      } else if (std::holds_alternative<float>(value)) {
        propertySet.setValue(key, std::get<float>(value));
      } else if (std::holds_alternative<bool>(value)) {
        propertySet.setValue(key, std::get<bool>(value));
      }
    }
    return propertySet;
  }

  //==============================================================================
  /**
   * @brief Applies a juce::PropertySet to the settings container.
   *
   * @param _propertySet Pointer to the property set to apply.
   *
   * @details
   * Updates all settings from the property set, converting types as needed.
   * Only updates settings that already exist in the container.
   */
  inline void applyPropertySet(juce::PropertySet* _propertySet)
  {
    for (auto& [key, storedValue] : settings) {
      if (_propertySet->containsKey(key)) {
        if (std::holds_alternative<juce::String>(storedValue)) {
          settings[key] = _propertySet->getValue(key);
        } else if (std::holds_alternative<juce::Colour>(storedValue)) {
          settings[key] = juce::Colour::fromString(_propertySet->getValue(key));
        } else if (std::holds_alternative<int>(storedValue)) {
          settings[key] = _propertySet->getValue(key).getIntValue();
        } else if (std::holds_alternative<float>(storedValue)) {
          settings[key] = _propertySet->getValue(key).getFloatValue();
        } else if (std::holds_alternative<bool>(storedValue)) {
          settings[key] = _propertySet->getBoolValue(key);
        }
      }
    }
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  // (none for this class)

  //==============================================================================
  // Other members
  std::map<juce::String, SettingValue> settings;
};

} // namespace configuration
} // namespace dmt