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
    dmt::gui::Panel::Layout layout = { 2, 5 };
    setLayout(layout);
    addAndMakeVisible(gainSlider);
  }

  inline const juce::String getName() noexcept
  {
    return "Channel " + juce::String(channel);
  }

  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    auto gainSliderPoint = this->getGridPoint(2, 2);
    gainSlider.setSize(100, 100);
    gainSlider.setCentrePosition(gainSliderPoint);
  }

private:
  const juce::String channel;
  dmt::gui::components::LinearSliderComponent gainSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
