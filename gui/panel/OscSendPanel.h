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
class OscSendPanel dmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;

public:
  OscSendPanel(juce::AudioProcessorValueTreeState & apvts,
               const juce::String channel)
    : Panel(juce::String("Channel " + channel))
    , channel(channel)
    , gainSlider(apvts,
                 juce::String("Gain"),
                 juce::String("osc1Send" + channel + "Gain"),
                 dmt::InfoUnit::Type::Gain)
    , panSlider(apvts,
                juce::String("Pan"),
                juce::String("osc1Send" + channel + "Pan"),
                dmt::InfoUnit::Type::Gain,
                LinearSliderType::Bipolar)
  {
    setLayout({ 3, 32 });
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(panSlider);
  }

  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    auto bounds = getLocalBounds();
    auto gainSliderPoint = this->getGridPoint(bounds, 2, 10);
    gainSlider.setSizeAndCentre(gainSliderPoint);
    auto panSliderPrimaryPoint = this->getGridPoint(bounds, 1, 26);
    auto panSliderSecundaryPoint = this->getGridPoint(bounds, 3, 26);
    panSlider.setBoundsByPoints(panSliderPrimaryPoint, panSliderSecundaryPoint);
  }

private:
  const juce::String channel;
  dmt::gui::components::RotarySliderComponent gainSlider;
  dmt::gui::components::LinearSliderComponent panSlider;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscSendPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
