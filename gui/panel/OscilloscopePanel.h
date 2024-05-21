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
                 LinearSliderOrientation::Horizontal)
  {
    addAndMakeVisible(oscilloscopeComponent);
  }

  void extendResize() noexcept override
  {
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;
    oscilloscopeComponent.setBounds(getLocalBounds().reduced(rawPadding));
  }

private:
  OscilloscopeComponent oscilloscopeComponent;
  LinearSlider zoomSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt