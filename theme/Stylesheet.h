#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace theme {
//==============================================================================
class Stylesheet
{
public:
  //============================================================================
  Stylesheet() = delete;
  //============================================================================
  Stylesheet(const Stylesheet& obj) = delete;
  //============================================================================
  static std::shared_ptr<Stylesheet> getInstance()
  {
    if (instance == nullptr) {
      instance = std::make_shared<Stylesheet>();
    }
    return instance;
  }
  //============================================================================
  void updateValue(const juce::String& name, const juce::var& value)
  {
    if (properties.containsKey(name)) {
      properties.setValue(name, value);
    }
  }
  //============================================================================
  bool getBool(const juce::String& name)
  {
    auto value = properties.getBoolValue(name);
    return value;
  }
  //============================================================================
  int getInt(const juce::String& name)
  {
    auto value = properties.getIntValue(name);
    return value;
  }
  //============================================================================
  float getFloat(const juce::String& name)
  {
    auto value = properties.getDoubleValue(name);
    return (float)value;
  }
  //============================================================================
  juce::Colour getColour(const juce::String& name)
  {
    auto rawValue = properties.getValue(name);
    auto colour = juce::Colour::fromString(rawValue);
    return colour;
  }
  //============================================================================
private:
  static std::shared_ptr<Stylesheet> instance;
  juce::PropertySet properties;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Stylesheet)
};
//============================================================================
} // namespace theme
} // namespace dmt