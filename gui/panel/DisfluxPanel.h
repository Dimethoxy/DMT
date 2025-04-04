#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "gui/component/DisfluxDisplayComponent.h"
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
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<float>;
  using DisfluxDisplayComponent = dmt::gui::component::DisfluxDisplayComponent;
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
  DisfluxPanel(juce::AudioProcessorValueTreeState& apvts,
               FifoAudioBuffer& oscilloscopeBuffer)
    : AbstractPanel("Oscilloscope", false)
    , display(oscilloscopeBuffer, apvts)
    , amountSlider(apvts,
                   juce::String("Amount"),
                   juce::String("DisfluxAmount"),
                   Unit::Type::DisfluxAmount,
                   RotarySliderType::Positive)
    , spreadSlider(apvts,
                   juce::String("Spread"),
                   juce::String("DisfluxSpread"),
                   Unit::Type::DisfluxSpread,
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
    , mixSlider(apvts,
                juce::String("Mix"),
                juce::String("DisfluxMix"),
                Unit::Type::DisfluxMix,
                RotarySliderType::Positive)
  {
    setLayout({ 22, 60 });

    addAndMakeVisible(display);
    addAndMakeVisible(amountSlider);
    addAndMakeVisible(spreadSlider);
    addAndMakeVisible(fequencySlider);
    addAndMakeVisible(pinchSlider);
    addAndMakeVisible(mixSlider);
  }

  //============================================================================
  void extendResize() noexcept override
  {
    auto bounds = getLocalBounds();

    const float padding = rawPadding * size;
    auto displayBounds = bounds.reduced(padding);
    const float displayHorizontalPadding = 100.0f;
    const float displayVerticalPadding = 57.0f;
    displayBounds.removeFromBottom(displayVerticalPadding * size);
    displayBounds.removeFromLeft(displayHorizontalPadding * size);
    displayBounds.removeFromRight(displayHorizontalPadding * size);
    display.setBounds(displayBounds);

    const int upperRotarySliderRow = 17;
    const int lowerRotarySliderRow = 43;
    const int linearSliderRow = 51;

    const int amountSliderCol = 3;
    const int leftFequencySliderCol = 6;
    const int rightFequencySliderCol = 17;
    const int pinchSliderCol = 20;

    const auto amountSliderPoint =
      this->getGridPoint(bounds, amountSliderCol, upperRotarySliderRow);
    amountSlider.setSizeAndCentre(amountSliderPoint);

    const auto spreadSliderPoint =
      this->getGridPoint(bounds, amountSliderCol, lowerRotarySliderRow);
    spreadSlider.setSizeAndCentre(spreadSliderPoint);

    const auto leftFequencySliderPoint =
      this->getGridPoint(bounds, leftFequencySliderCol, linearSliderRow);
    const auto rightFequencySliderPoint =
      this->getGridPoint(bounds, rightFequencySliderCol, linearSliderRow);
    fequencySlider.setBoundsByPoints(leftFequencySliderPoint,
                                     rightFequencySliderPoint);

    const auto pinchSliderPoint =
      this->getGridPoint(bounds, pinchSliderCol, lowerRotarySliderRow);
    pinchSlider.setSizeAndCentre(pinchSliderPoint);

    const auto mixSliderPoint =
      this->getGridPoint(bounds, pinchSliderCol, upperRotarySliderRow);
    mixSlider.setSizeAndCentre(mixSliderPoint);
  }
  //============================================================================
private:
  DisfluxDisplayComponent display;
  RotarySlider amountSlider;
  RotarySlider spreadSlider;
  LinearSlider fequencySlider;
  RotarySlider pinchSlider;
  RotarySlider mixSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisfluxPanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt