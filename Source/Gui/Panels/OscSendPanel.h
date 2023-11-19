//==============================================================================
#pragma once
//==============================================================================
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
                 juce::String("Gain"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Gain)
  {
    dmt::gui::Panel::Layout layout = { 1, 10 };
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
    auto bounds = getLocalBounds();
    auto gainSliderPoint = this->getGridPoint(bounds, 1, 5);
    gainSlider.setSizeAndCentre(gainSliderPoint);
  }

private:
  const juce::String channel;
  dmt::gui::components::RotarySliderComponent gainSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
