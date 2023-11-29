//==============================================================================
#pragma once
//==============================================================================
#include "../../utility/Unit.h"
#include "../components/RotarySliderComponent.h"
#include "Panel.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class WaveformDistortionPanel : public dmt::gui::Panel
{

  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;

public:
  WaveformDistortionPanel(juce::AudioProcessorValueTreeState& apvts)
    : Panel("Waveform Distortion")
    , typeSlider(apvts,
                 juce::String("Type"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::DistortionType)
    , gainSlider(apvts,
                 juce::String("Gain"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
    , driveSlider(apvts,
                  juce::String("Drive"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
  {
    setLayout({ 9, 10 });
    addAndMakeVisible(typeSlider);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(gainSlider);
  }
  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    auto bounds = getLocalBounds();
    auto typeSliderPoint = this->getGridPoint(bounds, 3, 5);
    typeSlider.setSizeAndCentre(typeSliderPoint);
    auto gainSliderPoint = this->getGridPoint(bounds, 5, 5);
    gainSlider.setSizeAndCentre(gainSliderPoint);
    auto driveSliderPoint = this->getGridPoint(bounds, 7, 5);
    driveSlider.setSizeAndCentre(driveSliderPoint);
  }

private:
  dmt::gui::components::RotarySliderComponent typeSlider;
  dmt::gui::components::RotarySliderComponent gainSlider;
  dmt::gui::components::RotarySliderComponent driveSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDistortionPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
