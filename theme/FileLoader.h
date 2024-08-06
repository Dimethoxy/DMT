#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <toml++/toml.hpp>
//==============================================================================
namespace dmt {
namespace theme {
class Loader
{
  using String = juce::String;
  using StyleNode = juce::HashMap<String, String>;
  using StyleTree = juce::HashMap<String, StyleNode>;

  static StyleTree loadTheme(const juce::String& path)
  {
    toml::table file;
    StyleTree tree;
    try {
      file = toml::parse_file(path.toStdString());
      for (auto [section, content] : file) {
        auto sectionStringView = section.str();
        auto sectionString =
          String(sectionStringView.data(), sectionStringView.size());
        if (content.is_table()) {
          StyleNode node;
          for (auto [key, value] : *content.as_table()) {
            auto keyStringView = key.str();
            auto keyString = String(keyStringView.data(), keyStringView.size());
            auto valueString = String(value.as_string()->get());
            node.set(keyString, valueString);
          }
          tree.set(sectionString, node);
        }
      }
    } catch (const toml::parse_error& err) {
      jassertfalse;
    }
  }
};
} // namespace theme
} // namespace dmt