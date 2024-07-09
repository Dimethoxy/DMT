//==============================================================================

#pragma once

#include "Panel.h"
#include "gui/component/LinearSliderComponent.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class AnalogPitchPanel : public dmt::gui::Panel
{
  using LinearSliderType = dmt::gui::widgets::LinearSlider::Type;
  using LinearSliderOrientation = dmt::gui::widgets::LinearSlider::Orientation;

public:
  AnalogPitchPanel(juce::AudioProcessorValueTreeState& apvts)
    : Panel("Analog Pitch Envelope")
    , attackSlider(apvts,
                   juce::String("Attack"),
                   juce::String("osc1PitchEnvAttack"),
                   dmt::InfoUnit::Type::Milliseconds,
                   LinearSliderType::Positive,
                   LinearSliderOrientation::Vertical)
    , decaySlider(apvts,
                  juce::String("Decay"),
                  juce::String("osc1PitchEnvDecay"),
                  dmt::InfoUnit::Type::Milliseconds,
                  LinearSliderType::Positive,
                  LinearSliderOrientation::Vertical)
    , skewSlider(apvts,
                 juce::String("Skew"),
                 juce::String("osc1PitchEnvSkew"),
                 dmt::InfoUnit::Type::Skew,
                 LinearSliderType::Positive,
                 LinearSliderOrientation::Vertical)
    , depthSlider(apvts,
                  juce::String("Depth"),
                  juce::String("osc1PitchEnvDepth"),
                  dmt::InfoUnit::Type::ModDepth,
                  LinearSliderType::Positive,
                  LinearSliderOrientation::Vertical)
  {
    setLayout({ 25, 32 });
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(skewSlider);
  }

  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    const auto bounds = getLocalBounds();

    const int primaryRow = 3;
    const int secundaryRow = 28;

    const int attackCol = 7;
    const int decayCol = 11;
    const int skewCol = 15;
    const int depthCol = 19;

    const auto attackSliderPrimaryPoint =
      this->getGridPoint(bounds, attackCol, primaryRow);
    const auto attackSliderSecondaryPoint =
      this->getGridPoint(bounds, attackCol, secundaryRow);
    attackSlider.setBoundsByPoints(attackSliderPrimaryPoint,
                                   attackSliderSecondaryPoint);

    const auto decaySliderPrimaryPoint =
      this->getGridPoint(bounds, decayCol, primaryRow);
    const auto decaySliderSecondaryPoint =
      this->getGridPoint(bounds, decayCol, secundaryRow);
    decaySlider.setBoundsByPoints(decaySliderPrimaryPoint,
                                  decaySliderSecondaryPoint);

    const auto skewSliderPrimaryPoint =
      this->getGridPoint(bounds, skewCol, primaryRow);
    const auto skewSliderSecondaryPoint =
      this->getGridPoint(bounds, skewCol, secundaryRow);
    skewSlider.setBoundsByPoints(skewSliderPrimaryPoint,
                                 skewSliderSecondaryPoint);

    const auto depthSliderPrimaryPoint =
      this->getGridPoint(bounds, depthCol, primaryRow);
    const auto depthSliderSecondaryPoint =
      this->getGridPoint(bounds, depthCol, secundaryRow);
    depthSlider.setBoundsByPoints(depthSliderPrimaryPoint,
                                  depthSliderSecondaryPoint);
  }

private:
  dmt::gui::components::LinearSliderComponent attackSlider;
  dmt::gui::components::LinearSliderComponent depthSlider;
  dmt::gui::components::LinearSliderComponent decaySlider;
  dmt::gui::components::LinearSliderComponent skewSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogPitchPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
