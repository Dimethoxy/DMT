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
    , biasSlider(apvts,
                 juce::String("Bias"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Symmetry)
    , crushSlider(apvts,
                  juce::String("Bitcrush"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
  {
    setLayout({ 18, 10 });
    addAndMakeVisible(typeSlider);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(biasSlider);
    addAndMakeVisible(crushSlider);
  }
  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    auto bounds = getLocalBounds();
    auto typeSliderPoint = this->getGridPoint(bounds, 6, 5);
    typeSlider.setSizeAndCentre(typeSliderPoint);
    auto gainSliderPoint = this->getGridPoint(bounds, 10, 5);
    gainSlider.setSizeAndCentre(gainSliderPoint);
    auto driveSliderPoint = this->getGridPoint(bounds, 14, 5);
    driveSlider.setSizeAndCentre(driveSliderPoint);
    auto biasSliderPrimaryPoint = this->getGridPoint(bounds, 2, 8);
    auto biasSliderSecondaryPoint = this->getGridPoint(bounds, 9, 8);
    biasSlider.setBoundsByPoints(biasSliderPrimaryPoint,
                                 biasSliderSecondaryPoint);
    auto crushSliderPrimaryPoint = this->getGridPoint(bounds, 11, 8);
    auto crushSliderSecondaryPoint = this->getGridPoint(bounds, 17, 8);
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
