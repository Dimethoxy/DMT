//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class VoicingPanel : public dmt::gui::Panel
{
  using RotarySliderComponent = dmt::gui::components::RotarySliderComponent;
  using LinearSliderComponent = dmt::gui::components::LinearSliderComponent;
  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;

public:
  VoicingPanel(juce::AudioProcessorValueTreeState& apvts)
    : Panel("Voices")
    , osctaveSlider(apvts,
                    juce::String("Osctave"),
                    juce::String("oscGain"),
                    dmt::InfoUnit::Type::Drive)
    , semitonesSlider(apvts,
                      juce::String("Semitones"),
                      juce::String("oscGain"),
                      dmt::InfoUnit::Type::Drive)
    , fineSlider(apvts,
                 juce::String("Symmetry"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Symmetry,
                 LinearSliderType::Bipolar)
    , densitySlider(apvts,
                    juce::String("Density"),
                    juce::String("oscGain"),
                    dmt::InfoUnit::Type::Drive)
    , detuneSlider(apvts,
                   juce::String("Detune"),
                   juce::String("oscGain"),
                   dmt::InfoUnit::Type::Drive)
    , phaseSlider(apvts,
                  juce::String("Phase"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Symmetry,
                  LinearSliderType::Bipolar)

  {
    setLayout({ 31, 32 });

    addAndMakeVisible(osctaveSlider);
    addAndMakeVisible(semitonesSlider);
    addAndMakeVisible(fineSlider);

    addAndMakeVisible(densitySlider);
    addAndMakeVisible(detuneSlider);
    addAndMakeVisible(phaseSlider);
  }
  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    const auto bounds = getLocalBounds();
    const int rotarySliderRow = 10;
    const int linearSliderRow = 26;

    //==============================================================================
    const int osctaveSliderCol = 3;
    const int semitoneSliderCol = 7;

    const int fineSliderPrimaryCol = 2;
    const int fineSliderSecondaryCol = 8;

    const auto osctaveSliderPoint =
      this->getGridPoint(bounds, osctaveSliderCol, rotarySliderRow);
    osctaveSlider.setSizeAndCentre(osctaveSliderPoint);

    const auto semitoneSliderPoint =
      this->getGridPoint(bounds, semitoneSliderCol, rotarySliderRow);
    semitonesSlider.setSizeAndCentre(semitoneSliderPoint);

    const auto fineSliderPrimaryPoint =
      this->getGridPoint(bounds, fineSliderPrimaryCol, linearSliderRow);
    const auto fineSliderSecondaryPoint =
      this->getGridPoint(bounds, fineSliderSecondaryCol, linearSliderRow);
    fineSlider.setBoundsByPoints(fineSliderPrimaryPoint,
                                 fineSliderSecondaryPoint);
    //==============================================================================

    const int densitySliderCol = 12;
    const int detuneSliderCol = 16;

    const int phaseSliderPrimaryCol = 11;
    const int phaseSliderSecondaryCol = 17;

    const auto densitySliderPoint =
      this->getGridPoint(bounds, densitySliderCol, rotarySliderRow);
    densitySlider.setSizeAndCentre(densitySliderPoint);

    const auto detuneSliderPoint =
      this->getGridPoint(bounds, detuneSliderCol, rotarySliderRow);
    detuneSlider.setSizeAndCentre(detuneSliderPoint);

    const auto phaseSliderPrimaryPoint =
      this->getGridPoint(bounds, phaseSliderPrimaryCol, linearSliderRow);
    const auto phaseSliderSecondaryPoint =
      this->getGridPoint(bounds, phaseSliderSecondaryCol, linearSliderRow);
    phaseSlider.setBoundsByPoints(phaseSliderPrimaryPoint,
                                  phaseSliderSecondaryPoint);
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoicingPanel)
  RotarySliderComponent osctaveSlider;
  RotarySliderComponent semitonesSlider;
  LinearSliderComponent fineSlider;

  RotarySliderComponent densitySlider;
  RotarySliderComponent detuneSlider;
  LinearSliderComponent phaseSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
