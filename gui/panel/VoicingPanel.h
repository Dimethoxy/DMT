//==============================================================================

#pragma once

#include "gui/panel/AbstractPanel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
//==============================================================================
class VoicingPanel dmt::gui::panel::AbstractPanel
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

  using RotarySliderComponent = dmt::gui::components::RotarySliderComponent;
  using LinearSliderComponent = dmt::gui::components::LinearSliderComponent;
  using RotarySliderType = dmt::gui::widgets::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;

public:
  VoicingPanel(juce::AudioProcessorValueTreeState & apvts)
    : Panel("Voices")
    , osctaveSlider(apvts,
                    juce::String("Octave"),
                    juce::String("osc1VoiceOctave"),
                    dmt::InfoUnit::Type::Octave,
                    RotarySliderType::Selector)
    , semitonesSlider(apvts,
                      juce::String("Semitone"),
                      juce::String("osc1VoiceSemitone"),
                      dmt::InfoUnit::Type::Semitone,
                      RotarySliderType::Selector)
    , fineSlider(apvts,
                 juce::String("Fine"),
                 juce::String("osc1VoiceFine"),
                 dmt::InfoUnit::Type::Fine,
                 LinearSliderType::Bipolar)
    , densitySlider(apvts,
                    juce::String("Density"),
                    juce::String("osc1VoiceDensity"),
                    dmt::InfoUnit::Type::Density,
                    RotarySliderType::Selector)
    , detuneSlider(apvts,
                   juce::String("Detune"),
                   juce::String("osc1VoiceDetune"),
                   dmt::InfoUnit::Type::Detune)
    , distributionSlider(apvts,
                         juce::String("Distibution"),
                         juce::String("osc1VoiceDistribution"),
                         dmt::InfoUnit::Type::Distribution)
    , widthSlider(apvts,
                  juce::String("Width"),
                  juce::String("osc1VoiceWidth"),
                  dmt::InfoUnit::Type::Percent,
                  LinearSliderType::Positive)
    , blendSlider(apvts,
                  juce::String("Bend"),
                  juce::String("osc1VoiceBlend"),
                  dmt::InfoUnit::Type::Percent,
                  LinearSliderType::Positive)
    , seedSlider(apvts,
                 juce::String("Seed"),
                 juce::String("osc1VoiceSeed"),
                 dmt::InfoUnit::Type::Seed,
                 RotarySliderType::Selector)
    , randomlider(apvts,
                  juce::String("Random"),
                  juce::String("osc1VoiceRandom"),
                  dmt::InfoUnit::Type::Percent)

    , phaseSlider(apvts,
                  juce::String("Phase"),
                  juce::String("osc1VoicePhase"),
                  dmt::InfoUnit::Type::Phase,
                  LinearSliderType::Positive)
  {
    setLayout({ 31, 32 });

    addAndMakeVisible(osctaveSlider);
    addAndMakeVisible(semitonesSlider);
    addAndMakeVisible(fineSlider);

    addAndMakeVisible(densitySlider);
    addAndMakeVisible(detuneSlider);
    addAndMakeVisible(distributionSlider);
    addAndMakeVisible(widthSlider);
    addAndMakeVisible(blendSlider);

    addAndMakeVisible(seedSlider);
    addAndMakeVisible(randomlider);
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
    const int distributionSliderCol = 20;

    const auto densitySliderPoint =
      this->getGridPoint(bounds, densitySliderCol, rotarySliderRow);
    densitySlider.setSizeAndCentre(densitySliderPoint);

    const auto detuneSliderPoint =
      this->getGridPoint(bounds, detuneSliderCol, rotarySliderRow);
    detuneSlider.setSizeAndCentre(detuneSliderPoint);

    const auto distributionSliderPoint =
      this->getGridPoint(bounds, distributionSliderCol, rotarySliderRow);
    distributionSlider.setSizeAndCentre(distributionSliderPoint);

    const int widthSliderPrimaryCol = 10;
    const int widthSliderSecondaryCol = 15;
    const int blendSliderPrimaryCol = 17;
    const int blendSliderSecondaryCol = 22;

    const auto blendSliderPrimaryPoint =
      this->getGridPoint(bounds, widthSliderPrimaryCol, linearSliderRow);
    const auto blendSliderSecondaryPoint =
      this->getGridPoint(bounds, widthSliderSecondaryCol, linearSliderRow);
    blendSlider.setBoundsByPoints(blendSliderPrimaryPoint,
                                  blendSliderSecondaryPoint);

    const auto widthSliderPrimaryPoint =
      this->getGridPoint(bounds, blendSliderPrimaryCol, linearSliderRow);
    const auto widthSliderSecondaryPoint =
      this->getGridPoint(bounds, blendSliderSecondaryCol, linearSliderRow);
    widthSlider.setBoundsByPoints(widthSliderPrimaryPoint,
                                  widthSliderSecondaryPoint);

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
  RotarySliderComponent distributionSlider;

  RotarySliderComponent seedSlider;
  RotarySliderComponent randomlider;

  LinearSliderComponent fineSlider;
  LinearSliderComponent widthSlider;
  LinearSliderComponent blendSlider;
  LinearSliderComponent phaseSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
