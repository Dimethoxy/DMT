//==============================================================================

#pragma once

#include "gui/component/AbstractDisplayComponent.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class DisfluxDisplayComponent : public AbstractDisplayComponent
{
public:
  //============================================================================
  DisfluxDisplayComponent(/*juce::AudioProcessorValueTreeState& apvts*/)
    : AbstractDisplayComponent()
  {
  }
  //============================================================================
  void repaintDisplay(
    juce::Graphics& g,
    const juce::Rectangle<int>& displayBounds) const noexcept override
  {
    TRACER("DisfluxDisplayComponent::repaintDisplay");
  }

private:
};
} // namespace components
} // namespace gui
} // namespace dmt