
#pragma once

#include <JuceHeader.h>

namespace dmt {
struct InfoUnit
{
  enum class Type
  {
    OscilloscopeZoom,
    OscilloscopeGain
  };

  static inline juce::String getString(dmt::InfoUnit::Type type, float value)
  {
    switch (type) {
      case dmt::InfoUnit::Type::OscilloscopeZoom:
        return { juce::String((int)value) + juce::String("%") };
        break;
      default:
        jassert(false);
        return { juce::String("ERROR") };
        break;
    }
  }
};
} // namespace dmt