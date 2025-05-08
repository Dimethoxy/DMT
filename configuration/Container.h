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
  //============================================================================
  // Aliases for convenience
  using String = juce::String;
  using Colour = juce::Colour;

  // Struct to hold optional min/max for numeric types
  struct NumericRange
  {
    std::optional<double> min;
    std::optional<double> max;
  };

public:
  //============================================================================
  /**
   * @brief Pseudo type for storing heterogeneous settings.
   *
   * @details
   * The possible types are: String, Colour, int, float, and bool.
   * Used for type-safe, runtime-checked configuration storage.
   */
  typedef std::variant<String, Colour, int, float, bool> SettingValue;

  //============================================================================
  /**
   * @brief Default constructor.
   */
  Container() = default;

  //============================================================================
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
  inline T& get(const String _name)
  {
    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (std::holds_alternative<T>(it->second.value)) {
        return std::get<T>(it->second.value);
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
  inline T& add(const String _name,
                const T _value,
                std::optional<double> min = std::nullopt,
                std::optional<double> max = std::nullopt)
  {
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, float> ||
                    std::is_same_v<T, String> || std::is_same_v<T, Colour> ||
                    std::is_same_v<T, bool>,
                  "Unsupported type for Container::add");

    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (!std::holds_alternative<T>(it->second.value)) {
        jassertfalse;
        throw std::runtime_error("Type mismatch for setting: " +
                                 _name.toStdString());
      }
      // Update min/max if numeric
      if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
        if (min)
          it->second.range.min = min;
        if (max)
          it->second.range.max = max;
      }
    } else {
      SettingEntry entry;
      entry.value = _value;
      if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
        entry.range.min = min;
        entry.range.max = max;
      }
      settings[_name] = entry;
    }
    return std::get<T>(settings[_name].value);
  }

  //==============================================================================
  /**
   * @brief Sets the optional min/max range for an existing numeric setting.
   *
   * @param _name The name of the setting.
   * @param min The optional minimum value.
   * @param max The optional maximum value.
   *
   * @details
   * This only applies to numeric types (int, float). For other types, this
   * function has no effect.
   */
  inline void setRange(const String& _name,
                       std::optional<double> min,
                       std::optional<double> max)
  {
    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (std::holds_alternative<int>(it->second.value) ||
          std::holds_alternative<float>(it->second.value)) {
        it->second.range.min = min;
        it->second.range.max = max;
      }
    }
  }

  //==============================================================================
  /**
   * @brief Gets the optional min/max range for a numeric setting.
   *
   * @param _name The name of the setting.
   * @return The NumericRange struct containing optional min and max values.
   *
   * @details
   * Returns an empty NumericRange if the setting is not numeric or not found.
   */
  inline NumericRange getRange(const String& _name) const
  {
    auto it = settings.find(_name);
    if (it != settings.end()) {
      if (std::holds_alternative<int>(it->second.value) ||
          std::holds_alternative<float>(it->second.value)) {
        return it->second.range;
      }
    }
    return {};
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
    for (const auto& [key, entry] : settings) {
      const auto& value = entry.value;
      if (std::holds_alternative<String>(value)) {
        propertySet.setValue(key, std::get<String>(value));
      } else if (std::holds_alternative<Colour>(value)) {
        propertySet.setValue(key, std::get<Colour>(value).toString());
      } else if (std::holds_alternative<int>(value)) {
        propertySet.setValue(key, std::get<int>(value));
      } else if (std::holds_alternative<float>(value)) {
        propertySet.setValue(key, std::get<float>(value));
      } else if (std::holds_alternative<bool>(value)) {
        propertySet.setValue(key, std::get<bool>(value));
      }
      // Optionally, could serialize min/max as well if needed
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
    for (auto& [key, entry] : settings) {
      auto& storedValue = entry.value;
      if (_propertySet->containsKey(key)) {
        if (std::holds_alternative<String>(storedValue)) {
          storedValue = _propertySet->getValue(key);
        } else if (std::holds_alternative<Colour>(storedValue)) {
          storedValue = Colour::fromString(_propertySet->getValue(key));
        } else if (std::holds_alternative<int>(storedValue)) {
          storedValue = _propertySet->getValue(key).getIntValue();
        } else if (std::holds_alternative<float>(storedValue)) {
          storedValue = _propertySet->getValue(key).getFloatValue();
        } else if (std::holds_alternative<bool>(storedValue)) {
          storedValue = _propertySet->getBoolValue(key);
        }
      }
    }
  }

  //==============================================================================
  /**
   * @brief Returns a mutable reference to the internal settings map.
   *
   * @details
   * This is used by adapters that need to provide mutable access to values.
   */
  inline std::map<String, SettingValue>& getAllSettingsMutable()
  {
    // For backward compatibility, return a map of just the values
    static std::map<String, SettingValue> legacy;
    legacy.clear();
    for (auto& [k, v] : settings)
      legacy[k] = v.value;
    return legacy;
  }

  //==============================================================================
private:
  // Each setting stores its value and optional numeric range
  struct SettingEntry
  {
    SettingValue value;
    NumericRange range;
  };

  std::map<String, SettingEntry> settings;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Container)
};
//==============================================================================
} // namespace configuration
} // namespace dmt
