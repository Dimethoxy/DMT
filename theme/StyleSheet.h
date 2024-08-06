#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <toml++/toml.hpp>
//==============================================================================
namespace dmt {
namespace theme {
using StyleNode = juce::HashMap<juce::String, juce::String>;
using StyleTree = juce::HashMap<juce::String, StyleNode>;
class Stylesheet
{
public:
  //============================================================================
  Stylesheet(const juce::String& path)
  {
    toml::table file;
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
  //============================================================================
  juce::Colour getColour(const juce::String& nodeName, const juce::String& key)
  {
    if (tree.contains(nodeName)) {
      auto node = tree[nodeName];
      if (node.contains(key)) {
        return juce::Colour::fromString(node[key]);
      }
    }
  }
  //============================================================================
  juce::String getString(const juce::String& nodeName, const juce::String& key)
  {
    if (tree.contains(nodeName)) {
      auto node = tree[nodeName];
      if (node.contains(key)) {
        return node[key];
      }
    }
  }
  //============================================================================
  bool getBool(const juce::String& nodeName, const juce::String& key)
  {
    if (tree.contains(nodeName)) {
      auto node = tree[nodeName];
      if (node.contains(key)) {
        return node[key] == "true";
      }
    }
  }
  //============================================================================
  int getInt(const juce::String& nodeName, const juce::String& key)
  {
    if (tree.contains(nodeName)) {
      auto node = tree[nodeName];
      if (node.contains(key)) {
        return node[key].getIntValue();
      }
    }
  }
  //============================================================================
  float getFloat(const juce::String& nodeName, const juce::String& key)
  {
    if (tree.contains(nodeName)) {
      auto node = tree[nodeName];
      if (node.contains(key)) {
        return node[key].getFloatValue();
      }
    }
  }
  //============================================================================
private:
  StyleTree tree;
};
} // namespace theme
} // namespace dmt