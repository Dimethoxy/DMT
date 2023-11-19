//==============================================================================

#pragma once

#include "../../Utility/Unit.h"
#include "../Components/LinearSliderComponent.h"
#include "../Components/RotarySliderComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class OscSendPanel : public dmt::gui::Panel
{
  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;

public:
  OscSendPanel(juce::AudioProcessorValueTreeState& apvts,
               const juce::String channel)
    : channel(channel)
    , gainSlider(apvts,
                 juce::String("oscSendGain"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Gain)
  {
    dmt::gui::Panel::Layout layout = { 1, 5 };
    setLayout(layout);
  }

  inline const juce::String getName() noexcept
  {
    return "Channel " + juce::String(channel);
  }

private:
  const juce::String channel;
  dmt::gui::components::LinearSliderComponent gainSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
