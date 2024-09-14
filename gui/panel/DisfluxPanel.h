#pragma once
//==============================================================================
#include "gui/panel/AbstractPanel.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
//==============================================================================
template<typename SampleType>
class DisfluxPanel
  : public dmt::gui::panel::AbstractPanel
  , public juce::Slider::Listener
{
  using LinearSlider = dmt::gui::component::LinearSliderComponent;
  using LinearSliderType = dmt::gui::widget::LinearSlider::Type;
  using LinearSliderOrientation = dmt::gui::widget::LinearSlider::Orientation;
  using Unit = dmt::utility::Unit;

  using Settings = dmt::Settings;
  const float& size = Settings::Window::size;
  const float& rawPadding = Settings::Panel::padding;

public:
  //============================================================================
  DisfluxPanel(juce::AudioProcessorValueTreeState& apvts)
    : AbstractPanel("Oscilloscope", false)
    , oscilloscopeComponent(fifoBuffer)
    , amountSlider(apvts,
                   juce::String("Amount"),
                   juce::String("DisfluxAmount"),
                   Unit::Type::DisfluxAmount,
                   LinearSliderType::Positive,
                   LinearSliderOrientation::Vertical,
                   true)
    , fequencySlider(apvts,
                     juce::String("Frequency"),
                     juce::String("DisfluxFrequency"),
                     Unit::Type::DisfluxFrequency,
                     LinearSliderType::Positive,
                     LinearSliderOrientation::Horizontal,
                     true)
    , pinchSlider(apvts,
                  juce::String("Pinch"),
                  juce::String("DisfluxPinch"),
                  Unit::Type::DisfluxPinch,
                  LinearSliderType::Bipolar,
                  LinearSliderOrientation::Vertical,
                  true)
  {
    addAndMakeVisible(oscilloscopeComponent);
    addAndMakeVisible(amountSlider);
    addAndMakeVisible(fequencySlider);
    addAndMakeVisible(pinchSlider);
    amountSlider.getSlider().addListener(this);
    fequencySlider.getSlider().addListener(this);
    pinchSlider.getSlider().addListener(this);
  }
  //============================================================================
  void extendResize() noexcept override
  {
    const auto padding = rawPadding * size;
    auto bounds = getLocalBounds().reduced(padding);

    const auto sliderWidth = 38 * size;
  }
  //============================================================================
  void sliderValueChanged(juce::Slider* slider) override {}
  //============================================================================
private:
  LinearSlider amountSlider;
  LinearSlider fequencySlider;
  LinearSlider pinchSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisfluxPanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt