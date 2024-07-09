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
class WaveformDistortionPaneldmt::gui::panel::AbstractPanel;
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;

public:
  WaveformDistortionPanel(juce::AudioProcessorValueTreeState & apvts)
    : Panel("Waveform Distortion")
    , typeSlider(apvts,
                 juce::String("Type"),
                 juce::String("osc1DistortionType"),
                 dmt::InfoUnit::Type::DistortionType,
                 RotarySliderType::Selector)
    , gainSlider(apvts,
                 juce::String("Pregain"),
                 juce::String("osc1DistortionPreGain"),
                 dmt::InfoUnit::Type::Gain)
    , driveSlider(apvts,
                  juce::String("Drive"),
                  juce::String("osc1DistortionDrive"),
                  dmt::InfoUnit::Type::Drive)
    , biasSlider(apvts,
                 juce::String("Symmetry"),
                 juce::String("osc1DistortionSymmetry"),
                 dmt::InfoUnit::Type::Symmetry,
                 LinearSliderType::Bipolar)
    , crushSlider(apvts,
                  juce::String("Bitcrush"),
                  juce::String("osc1DistortionCrush"),
                  dmt::InfoUnit::Type::Bitcrush)
  {
    setLayout({ 17, 32 });
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
    const int linearSliderRow = 26;

    const int typeSliderCol = 4;
    const int gainSliderCol = 9;
    const int driveSliderCol = 14;

    const int biasPrimaryCol = 2;
    const int biasSecondaryCol = 8;
    const int crushPrimaryCol = 10;
    const int crushSecondaryCol = 16;

    const auto typeSliderPoint =
      this->getGridPoint(bounds, typeSliderCol, rotarySliderRow);
    typeSlider.setSizeAndCentre(typeSliderPoint);

    const auto gainSliderPoint =
      this->getGridPoint(bounds, gainSliderCol, rotarySliderRow);
    gainSlider.setSizeAndCentre(gainSliderPoint);

    const auto driveSliderPoint =
      this->getGridPoint(bounds, driveSliderCol, rotarySliderRow);
    driveSlider.setSizeAndCentre(driveSliderPoint);

    const auto biasSliderPrimaryPoint =
      this->getGridPoint(bounds, biasPrimaryCol, linearSliderRow);
    const auto biasSliderSecondaryPoint =
      this->getGridPoint(bounds, biasSecondaryCol, linearSliderRow);
    biasSlider.setBoundsByPoints(biasSliderPrimaryPoint,
                                 biasSliderSecondaryPoint);

    const auto crushSliderPrimaryPoint =
      this->getGridPoint(bounds, crushPrimaryCol, linearSliderRow);
    const auto crushSliderSecondaryPoint =
      this->getGridPoint(bounds, crushSecondaryCol, linearSliderRow);
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
