#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace processor {
class PluginProcessor
{
public:
  PluginProcessor()
    : apvts(_apvts)
  {
  }
  ~PluginProcessor() = default;
  juce::AudioProcessorValueTreeState apvts;
};
}
}
}