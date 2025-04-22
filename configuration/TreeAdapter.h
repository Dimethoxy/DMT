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
 * Adapter for browsing settings as a fixed-depth category/leaf tree.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include "configuration/Container.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace configuration {

//==============================================================================
/**
 * @brief Adapter for browsing settings as a category/leaf tree.
 *
 * @details
 * This adapter provides a fixed-depth (category/leaf) view of a flat
 * dmt::configuration::Container. It is intended for use in GUIs or
 * other tools that require hierarchical browsing of settings.
 *
 * The adapter groups settings by splitting their names at the first '.'
 * character, treating the prefix as the category and the suffix as the leaf.
 * Only settings with at least one '.' are included in the tree.
 *
 * The adapter holds a reference to the underlying Container, so all
 * mutations are reflected in real time. Call rebuild() if settings are
 * added or removed after construction.
 */
class TreeAdapter
{
  using Container = dmt::configuration::Container;

public:
  //==============================================================================
  /**
   * @brief Represents a leaf node in the settings tree.
   *
   * @details
   * Each leaf corresponds to a single setting value within a category.
   * The value pointer is always valid as long as the Container exists.
   */
  struct Leaf
  {
    juce::String name;
    const Container::SettingValue* value;
  };

  //==============================================================================
  /**
   * @brief Represents a category node in the settings tree.
   *
   * @details
   * Each category contains a list of leaves (settings) that share the same
   * category prefix.
   */
  struct Category
  {
    juce::String name;
    std::vector<Leaf> leaves;
  };

  //==============================================================================
  /**
   * @brief Constructs the adapter with a reference to a Container.
   *
   * @param _containerRef Reference to the settings container to adapt.
   *
   * @details
   * The adapter does not take ownership. All mutations to the container
   * are reflected in the adapter. Call rebuild() if the set of settings
   * changes after construction.
   */
  inline explicit TreeAdapter(Container& _containerRef) noexcept
    : container(_containerRef)
  {
    buildTree();
  }

  //==============================================================================
  /**
   * @brief Rebuilds the category/leaf tree from the container.
   *
   * @details
   * Call this after adding or removing settings in the container.
   * This operation is not real-time safe and should not be called from
   * the audio thread.
   */
  inline void rebuild() noexcept { buildTree(); }

  //==============================================================================
  /**
   * @brief Returns the list of categories, each with its leaves.
   *
   * @return Const reference to the vector of categories.
   *
   * @details
   * The returned reference remains valid until rebuild() is called.
   */
  [[nodiscard]] inline const std::vector<Category>& getCategories()
    const noexcept
  {
    return categories;
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  Container& container;

  //==============================================================================
  // Other members
  std::vector<Category> categories;

  //==============================================================================
  /**
   * @brief Builds the category/leaf tree from the container.
   *
   * @details
   * Groups all settings by their category prefix (before the first '.').
   * Only settings with a '.' in their name are included.
   */
  inline void buildTree() noexcept
  {
    categories.clear();
    std::map<juce::String, std::vector<Leaf>> categoryMap;

    for (const auto& [key, value] : container.getAllSettings()) {
      auto dotIndex = static_cast<int>(key.indexOfChar('.'));
      if (dotIndex < 0)
        continue;
      juce::String category = key.substring(0, dotIndex);
      juce::String leaf = key.substring(dotIndex + 1);
      categoryMap[category].push_back(Leaf{ leaf, &value });
    }

    for (auto& [cat, leaves] : categoryMap) {
      categories.push_back(Category{ cat, leaves });
    }
  }

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreeAdapter)
};
} // namespace configuration
} // namespace dmt