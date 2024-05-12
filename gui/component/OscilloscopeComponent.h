#pragma once
//==============================================================================
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class OscilloscopeComponent : public juce::Component
{
public:
  //==============================================================================
  OscilloscopeComponent();
  ~OscilloscopeComponent() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

  //==============================================================================
private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt