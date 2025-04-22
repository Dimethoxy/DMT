#pragma once

#include "configuration/Container.h"
#include <JuceHeader.h>

//=============================================================================
// Adapter for browsing settings as a category/leaf tree.
class SettingsTreeAdapter
{
  using Container = dmt::configuration::Container;

public:
  struct Leaf
  {
    juce::String name;                    // e.g. "BackgroundColour"
    const Container::SettingValue* value; // pointer to value in container
  };

  struct Category
  {
    juce::String name; // e.g. "Window"
    std::vector<Leaf> leaves;
  };

  // Construct with a reference to a Container
  SettingsTreeAdapter(Container& containerRef)
    : container(containerRef)
  {
    buildTree();
  }

  // Rebuilds the tree (call if settings are added/removed)
  void rebuild() { buildTree(); }

  // Returns the list of categories (each with its leaves)
  const std::vector<Category>& getCategories() const { return categories; }

private:
  Container& container;
  std::vector<Category> categories;

  void buildTree()
  {
    categories.clear();
    std::map<juce::String, std::vector<Leaf>> categoryMap;

    // Group settings by category (split at first '.')
    for (auto& [key, value] : container.getAllSettings()) {
      auto dotIndex = key.indexOfChar('.');
      if (dotIndex < 0)
        continue; // skip non-categorized
      juce::String category = key.substring(0, dotIndex);
      juce::String leaf = key.substring(dotIndex + 1);
      categoryMap[category].push_back(Leaf{ leaf, &value });
    }

    // Build categories vector
    for (auto& [cat, leaves] : categoryMap) {
      categories.push_back(Category{ cat, leaves });
    }
  }
};

} // namespace configuration
} // namespace dmt