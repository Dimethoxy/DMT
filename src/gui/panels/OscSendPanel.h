//==============================================================================
#pragma once
//==============================================================================
#include "../../utility/Unit.h"
#include "../components/LinearSliderComponent.h"
#include "../components/RotarySliderComponent.h"
#include "Panel.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class OscSendPanel : public dmt::gui::Panel {
  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;

public:
  OscSendPanel(juce::AudioProcessorValueTreeState &apvts,
               const juce::String channel)
      : Panel(juce::String("Channel " + channel)), channel(channel),
        gainSlider(apvts, juce::String("Distortion"), juce::String("oscGain"),
                   dmt::InfoUnit::Type::Gain) {
    dmt::gui::Panel::Layout layout = {1, 10};
    setLayout(layout);
    addAndMakeVisible(gainSlider);
  }

  void resized() noexcept override {
    dmt::gui::Panel::resized();
    auto bounds = getLocalBounds();
    auto gainSliderPoint = this->getGridPoint(bounds, 1, 5);
    gainSlider.setSizeAndCentre(gainSliderPoint);
  }

private:
  const juce::String channel;
  dmt::gui::components::RotarySliderComponent gainSlider;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscSendPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
