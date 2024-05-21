#pragma once
//==============================================================================
#include "gui/component/LinearSliderComponent.h"
#include "gui/component/OscilloscopeComponent.h"
#include "gui/panel/AbstractPanel.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
template<typename SampleType>
class OscilloscopePanel : public AbstractPanel
{
  using OscilloscopeComponent =
    dmt::gui::component::OscilloscopeComponent<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;

  using LinearSlider = dmt::gui::component::LinearSliderComponent;
  using LinearSliderType = dmt::gui::widget::LinearSlider::Type;
  using LinearSliderOrientation = dmt::gui::widget::LinearSlider::Orientation;

  using Settings = dmt::Settings;
  const float& size = Settings::Layout::size;
  const float& rawPadding = Settings::Panel::padding;

public:
  OscilloscopePanel(FifoAudioBuffer& fifoBuffer,
                    juce::AudioProcessorValueTreeState& apvts)
    : AbstractPanel("Oscilloscope", false)
    , oscilloscopeComponent(fifoBuffer)
    , zoomSlider(apvts,
                 juce::String("Zoom"),
                 juce::String("OscilloscopeZoom"),
                 dmt::InfoUnit::Type::OscilloscopeZoom,
                 LinearSliderType::Positive,
                 LinearSliderOrientation::Vertical)
  {
    addAndMakeVisible(oscilloscopeComponent);
    addAndMakeVisible(zoomSlider);
  }

  void extendResize() noexcept override
  {
    const auto padding = rawPadding * size;
    auto bounds = getLocalBounds().reduced(padding);

    const auto sliderWidth = 30 * size;

    auto leftSliderBounds = bounds.removeFromLeft(sliderWidth);
    const auto leftSliderOffsetX = 5.0f * size;
    leftSliderBounds =
      leftSliderBounds.withX(leftSliderBounds.getX() + leftSliderOffsetX);
    zoomSlider.setBounds(leftSliderBounds);

    oscilloscopeComponent.setBounds(bounds);
  }

private:
  OscilloscopeComponent oscilloscopeComponent;
  LinearSlider zoomSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt