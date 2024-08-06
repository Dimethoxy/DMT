#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace theme {
class Stylesheet
{
public:
  Stylesheet() {}
  void updateValue(const juce::String& name, const juce::var& value)
  {
    if (properties.containsKey(name)) {
      properties.setValue(name, value);
    }
  }

  bool getBool(const juce::String& name)
  {
    auto value = properties.getBoolValue(name);
    return value;
  }

  int getInt(const juce::String& name)
  {
    auto value = properties.getIntValue(name);
    return value;
  }

  float getFloat(const juce::String& name)
  {
    auto value = properties.getDoubleValue(name);
    return (float)value;
  }

  juce::Colour getColour(const juce::String& name)
  {
    auto rawValue = properties.getValue(name);
    auto colour = juce::Colour::fromString(rawValue);
    return colour;
  }

private:
  juce::PropertySet properties;
};
} // namespace theme
} // namespace dmt