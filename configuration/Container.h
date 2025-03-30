#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace configuration {

/**
 * @class Container
 * @brief A class for managing a collection of settings with type safety.
 *
 * This class allows adding and retrieving settings with different types. It
 * ensures that the types of the settings are consistent, and provides type-safe
 * access to the values.
 */
class Container
{
  /**
   * @brief A variant type that can store values of different types.
   *
   * The possible types are: juce::String, juce::Colour, int, float, and bool.
   */
  using SettingValue =
    std::variant<juce::String, juce::Colour, int, float, bool>;

public:
  /**
   * @brief Retrieves a setting by its name with type safety.
   *
   * @tparam T The type of the setting to retrieve.
   * @param name The name of the setting.
   * @return A mutable reference to the setting value of type T.
   *
   * @throws std::runtime_error If the setting is not found or the type does not
   *         match.
   */
  template<typename T>
  T& get(const juce::String name)
  {
    auto it = settings.find(name);
    if (it != settings.end()) {
      if (std::holds_alternative<T>(it->second)) {
        return std::get<T>(
          it->second); // Return mutable reference if types match.
      } else {
        jassertfalse; // Assertion for type mismatch, debugging aid.
        throw std::runtime_error("Type mismatch for setting: " +
                                 name.toStdString());
      }
    } else {
      jassertfalse; // Assertion if setting is not found, debugging aid.
      throw std::runtime_error("Setting not found: " + name.toStdString());
    }
  }

  /**
   * @brief Adds a new setting or ensures consistency for an existing one.
   *
   * If the setting already exists, the type must match the stored type.
   * If it doesn't match, an error is thrown.
   * If the setting doesn't exist, it is added to the collection.
   *
   * @tparam T The type of the setting to add.
   * @param name The name of the setting.
   * @param value The value to associate with the setting.
   *
   * @throws std::runtime_error If there is a type mismatch when adding an
   *         existing setting.
   */
  template<typename T>
  T& add(const juce::String name, const T value)
  {
    auto it = settings.find(name);
    if (it != settings.end()) {
      if (!std::holds_alternative<T>(it->second)) {
        jassertfalse; // Assertion for type mismatch on existing setting,
                      // debugging aid.
        throw std::runtime_error("Type mismatch for setting: " +
                                 name.toStdString());
      }
    } else {
      settings[name] = value; // Add new setting if not already present.
    }

    return std::get<T>(settings[name]); // Return mutable reference.
  }

private:
  /**
   * @brief A map that stores settings by name.
   *
   * The map uses the setting name as the key and stores the value in a variant
   * type, allowing different types of settings to be stored in the same
   * container.
   */
  std::map<juce::String, SettingValue> settings;
};

} // namespace configuration
} // namespace dmt