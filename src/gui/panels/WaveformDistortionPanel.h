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
class WaveformDistortionPanel : public dmt::gui::Panel
{

  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;

public:
  WaveformDistortionPanel(juce::AudioProcessorValueTreeState& apvts)
    : Panel("Waveform Distortion")
    , typeSlider(apvts,
                 juce::String("Type"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
    , gainSlider(apvts,
                 juce::String("Pregain"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
    , driveSlider(apvts,
                  juce::String("Drive"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
    , biasSlider(apvts,
                 juce::String("Symmetry"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Symmetry,
                 LinearSliderType::Bipolar)
    , crushSlider(apvts,
                  juce::String("Bitcrush"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
  {
    setLayout({ 18, 32 });
    addAndMakeVisible(typeSlider);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(biasSlider);
    addAndMakeVisible(crushSlider);
  }
  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    const auto bounds = getLocalBounds();
    const int rotarySliderRow = 10;
    const int linearSliderRow = 23;
    const auto typeSliderPoint = this->getGridPoint(bounds, 5, rotarySliderRow);
    typeSlider.setSizeAndCentre(typeSliderPoint);
    const auto gainSliderPoint =
      this->getGridPoint(bounds, 10, rotarySliderRow);
    gainSlider.setSizeAndCentre(gainSliderPoint);
    const auto driveSliderPoint =
      this->getGridPoint(bounds, 15, rotarySliderRow);
    driveSlider.setSizeAndCentre(driveSliderPoint);
    const auto biasSliderPrimaryPoint =
      this->getGridPoint(bounds, 2, linearSliderRow);
    const auto biasSliderSecondaryPoint =
      this->getGridPoint(bounds, 9, linearSliderRow);
    biasSlider.setBoundsByPoints(biasSliderPrimaryPoint,
                                 biasSliderSecondaryPoint);
    const auto crushSliderPrimaryPoint =
      this->getGridPoint(bounds, 11, linearSliderRow);
    const auto crushSliderSecondaryPoint =
      this->getGridPoint(bounds, 17, linearSliderRow);
    crushSlider.setBoundsByPoints(crushSliderPrimaryPoint,
                                  crushSliderSecondaryPoint);
  }

private:
  dmt::gui::components::RotarySliderComponent typeSlider;
  dmt::gui::components::RotarySliderComponent gainSlider;
  dmt::gui::components::RotarySliderComponent driveSlider;
  dmt::gui::components::LinearSliderComponent biasSlider;
  dmt::gui::components::LinearSliderComponent crushSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDistortionPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
