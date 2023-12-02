//==============================================================================

#pragma once

#include "../components/LinearSliderComponent.h"
#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class AnalogPitchPanel : public dmt::gui::Panel
{
public:
  AnalogPitchPanel(juce::AudioProcessorValueTreeState& apvts)
    : Panel("Analog Pitch Envelope")
    , attackSlider(apvts,
                   juce::String("Attack"),
                   juce::String("oscGain"),
                   dmt::InfoUnit::Type::Drive)
    , holdSlider(apvts,
                 juce::String("Hold"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
    , decaySlider(apvts,
                  juce::String("Decay"),
                  juce::String("oscGain"),
                  dmt::InfoUnit::Type::Drive)
    , skewSlider(apvts,
                 juce::String("Skew"),
                 juce::String("oscGain"),
                 dmt::InfoUnit::Type::Drive)
  {
    setLayout({ 4, 32 });
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(holdSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(skewSlider);
  }

  void resized() noexcept override
  {
    dmt::gui::Panel::resized();
    const auto bounds = getLocalBounds();
    const int primaryRow = 4;
    const int secundaryRow = 25;

    const int attackCol = 1;
    const auto attackSliderPrimaryPoint =
      this->getGridPoint(bounds, attackCol, primaryRow);
    const auto attackSliderSecondaryPoint =
      this->getGridPoint(bounds, attackCol, secundaryRow);
    attackSlider.setBoundsByPoints(attackSliderPrimaryPoint,
                                   attackSliderSecondaryPoint);

    const int holdCol = 2;
    const auto holdSliderPrimaryPoint =
      this->getGridPoint(bounds, holdCol, primaryRow);
    const auto holdSliderSecondaryPoint =
      this->getGridPoint(bounds, holdCol, secundaryRow);
    holdSlider.setBoundsByPoints(holdSliderPrimaryPoint,
                                 holdSliderSecondaryPoint);

    const int decayCol = 3;
    const auto decaySliderPrimaryPoint =
      this->getGridPoint(bounds, decayCol, primaryRow);
    const auto decaySliderSecondaryPoint =
      this->getGridPoint(bounds, decayCol, secundaryRow);
    decaySlider.setBoundsByPoints(decaySliderPrimaryPoint,
                                  decaySliderSecondaryPoint);

    const int skewCol = 4;
    const auto skewSliderPrimaryPoint =
      this->getGridPoint(bounds, skewCol, primaryRow);
    const auto skewSliderSecondaryPoint =
      this->getGridPoint(bounds, skewCol, secundaryRow);
    skewSlider.setBoundsByPoints(skewSliderPrimaryPoint,
                                 skewSliderSecondaryPoint);
  }

private:
  dmt::gui::components::LinearSliderComponent attackSlider;
  dmt::gui::components::LinearSliderComponent holdSlider;
  dmt::gui::components::LinearSliderComponent decaySlider;
  dmt::gui::components::LinearSliderComponent skewSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogPitchPanel)
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
