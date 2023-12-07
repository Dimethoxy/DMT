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
    , densitySlider(apvts,
                    juce::String("Density"),
                    juce::String("oscGain"),
                    dmt::InfoUnit::Type::Drive)
    , detuneSlider(apvts,
                   juce::String("Detune"),
                   juce::String("oscGain"),
                   dmt::InfoUnit::Type::Drive)
    , blendSlider(apvts,
                  juce::String("Blend"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
    , seedSlider(apvts,
                 juce::String("Seed"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
    , randomlider(apvts,
                  juce::String("Random"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
    , fineSlider(apvts,
                 juce::String("Symmetry"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Symmetry,
                 LinearSliderType::Bipolar)
    , phaseSlider(apvts,
                  juce::String("Phase"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Symmetry,
                  LinearSliderType::Bipolar)

  {
    setLayout({ 31, 32 });

    addAndMakeVisible(osctaveSlider);
    addAndMakeVisible(semitonesSlider);

    addAndMakeVisible(densitySlider);
    addAndMakeVisible(detuneSlider);
    addAndMakeVisible(blendSlider);

    addAndMakeVisible(seedSlider);
    addAndMakeVisible(randomlider);

    addAndMakeVisible(fineSlider);
    addAndMakeVisible(phaseSlider);
  }
  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    const auto bounds = getLocalBounds();
    const int rotarySliderRow = 10;
    const int linearSliderRow = 26;

    //==============================================================================
    // Left Layout
    const int osctaveSliderCol = 3;
    const int semitoneSliderCol = 7;

    const auto osctaveSliderPoint =
      this->getGridPoint(bounds, osctaveSliderCol, rotarySliderRow);
    osctaveSlider.setSizeAndCentre(osctaveSliderPoint);

    const auto semitoneSliderPoint =
      this->getGridPoint(bounds, semitoneSliderCol, rotarySliderRow);
    semitonesSlider.setSizeAndCentre(semitoneSliderPoint);

    const int fineSliderPrimaryCol = 2;
    const int fineSliderSecondaryCol = 8;

    const auto fineSliderPrimaryPoint =
      this->getGridPoint(bounds, fineSliderPrimaryCol, linearSliderRow);
    const auto fineSliderSecondaryPoint =
      this->getGridPoint(bounds, fineSliderSecondaryCol, linearSliderRow);
    fineSlider.setBoundsByPoints(fineSliderPrimaryPoint,
                                 fineSliderSecondaryPoint);
    //==============================================================================
    // Middle Layout
    const int densitySliderCol = 12;
    const int detuneSliderCol = 16;
    const int blendSliderCol = 20;

    const auto densitySliderPoint =
      this->getGridPoint(bounds, densitySliderCol, rotarySliderRow);
    densitySlider.setSizeAndCentre(densitySliderPoint);

    const auto detuneSliderPoint =
      this->getGridPoint(bounds, detuneSliderCol, rotarySliderRow);
    detuneSlider.setSizeAndCentre(detuneSliderPoint);

    const auto blendSliderPoint =
      this->getGridPoint(bounds, blendSliderCol, rotarySliderRow);
    blendSlider.setSizeAndCentre(blendSliderPoint);

    //==============================================================================
    // Right Layout
    const int seedSliderCol = 25;
    const int randomSliderCol = 29;

    const auto seedSliderPoint =
      this->getGridPoint(bounds, seedSliderCol, rotarySliderRow);
    seedSlider.setSizeAndCentre(seedSliderPoint);

    const auto randomSliderPoint =
      this->getGridPoint(bounds, randomSliderCol, rotarySliderRow);
    randomlider.setSizeAndCentre(randomSliderPoint);

    const int phaseSliderPrimaryCol = 24;
    const int phaseSliderSecondaryCol = 30;

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

  RotarySliderComponent densitySlider;
  RotarySliderComponent detuneSlider;
  RotarySliderComponent blendSlider;

  RotarySliderComponent seedSlider;
  RotarySliderComponent randomlider;

  LinearSliderComponent fineSlider;
  LinearSliderComponent phaseSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
