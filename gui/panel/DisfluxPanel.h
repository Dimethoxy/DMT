#pragma once
//==============================================================================
#include "gui/component/LinearSliderComponent.h"
#include "gui/component/RotarySliderComponent.h"
#include "gui/panel/AbstractPanel.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
//==============================================================================
template<typename SampleType>
class DisfluxPanel : public dmt::gui::panel::AbstractPanel
{
  using LinearSlider = dmt::gui::component::LinearSliderComponent;
  using LinearSliderType = dmt::gui::widget::LinearSlider::Type;
  using LinearSliderOrientation = dmt::gui::widget::LinearSlider::Orientation;
  using RotarySlider = dmt::gui::component::RotarySliderComponent;
  using RotarySliderType = dmt::gui::widget::RotarySlider::Type;
  using Unit = dmt::utility::Unit;

  using Settings = dmt::Settings;
  const float& size = Settings::Window::size;
  const float& rawPadding = Settings::Panel::padding;

public:
  //============================================================================
  DisfluxPanel(juce::AudioProcessorValueTreeState& apvts)
    : AbstractPanel("Oscilloscope", false)
    , amountSlider(apvts,
                   juce::String("Amount"),
                   juce::String("DisfluxAmount"),
                   Unit::Type::DisfluxAmount,
                   RotarySliderType::Positive)
    , fequencySlider(apvts,
                     juce::String("Frequency"),
                     juce::String("DisfluxFrequency"),
                     Unit::Type::DisfluxFrequency,
                     LinearSliderType::Positive,
                     LinearSliderOrientation::Horizontal)
    , pinchSlider(apvts,
                  juce::String("Pinch"),
                  juce::String("DisfluxPinch"),
                  Unit::Type::DisfluxPinch,
                  RotarySliderType::Positive)
  {
    setLayout({ 28, 5 });

    addAndMakeVisible(amountSlider);
    addAndMakeVisible(fequencySlider);
    addAndMakeVisible(pinchSlider);
  }
  //============================================================================
  void extendResize() noexcept override
  {
    auto bounds = getLocalBounds();

    const int rotarySliderRow = 3;
    const int linearSliderRow = 5;

    const int amountSliderCol = 5;
    const int leftFequencySliderCol = 8;
    const int rightFequencySliderCol = 21;
    const int pinchSliderCol = 24;

    const auto amountSliderPoint =
      this->getGridPoint(bounds, amountSliderCol, rotarySliderRow);
    amountSlider.setSizeAndCentre(amountSliderPoint);

    const auto leftFequencySliderPoint =
      this->getGridPoint(bounds, leftFequencySliderCol, linearSliderRow);
    const auto rightFequencySliderPoint =
      this->getGridPoint(bounds, rightFequencySliderCol, linearSliderRow);
    fequencySlider.setBoundsByPoints(leftFequencySliderPoint,
                                     rightFequencySliderPoint);

    const auto pinchSliderPoint =
      this->getGridPoint(bounds, pinchSliderCol, rotarySliderRow);
    pinchSlider.setSizeAndCentre(pinchSliderPoint);
  }
  //============================================================================
private:
  RotarySlider amountSlider;
  LinearSlider fequencySlider;
  RotarySlider pinchSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisfluxPanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt