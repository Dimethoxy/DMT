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
//==============================================================================
template<typename SampleType>
class OscilloscopePanel
  : public dmt::gui::panel::AbstractPanel
  , public juce::Slider::Listener
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;

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
  //============================================================================
  OscilloscopePanel(FifoAudioBuffer& fifoBuffer,
                    juce::AudioProcessorValueTreeState& apvts)
    : AbstractPanel("Oscilloscope", false)
    , oscilloscopeComponent(fifoBuffer)
    , zoomSlider(apvts,
                 juce::String("Zoom"),
                 juce::String("OscilloscopeZoom"),
                 dmt::InfoUnit::Type::OscilloscopeZoom,
                 LinearSliderType::Positive,
                 LinearSliderOrientation::Vertical,
                 true)
    , thicknessSlider(apvts,
                      juce::String("Thickness"),
                      juce::String("OscilloscopeThickness"),
                      dmt::InfoUnit::Type::OscilloscopeThickness,
                      LinearSliderType::Positive,
                      LinearSliderOrientation::Vertical,
                      true)
    , heightSlider(apvts,
                   juce::String("Gain"),
                   juce::String("OscilloscopeGain"),
                   dmt::InfoUnit::Type::OscilloscopeHeight,
                   LinearSliderType::Bipolar,
                   LinearSliderOrientation::Vertical,
                   true)
  {
    addAndMakeVisible(oscilloscopeComponent);
    addAndMakeVisible(zoomSlider);
    addAndMakeVisible(thicknessSlider);
    addAndMakeVisible(heightSlider);
    zoomSlider.getSlider().addListener(this);
    thicknessSlider.getSlider().addListener(this);
    heightSlider.getSlider().addListener(this);
    sliderValueChanged(dynamic_cast<juce::Slider*>(&zoomSlider.getSlider()));
    sliderValueChanged(
      dynamic_cast<juce::Slider*>(&thicknessSlider.getSlider()));
    sliderValueChanged(dynamic_cast<juce::Slider*>(&heightSlider.getSlider()));
  }
  //============================================================================
  void extendResize() noexcept override
  {
    const auto padding = rawPadding * size;
    auto bounds = getLocalBounds().reduced(padding);

    const auto sliderWidth = 38 * size;

    auto leftSliderBounds = bounds.removeFromLeft(sliderWidth);
    const auto sliderOffsetX = 4.0f * size;
    leftSliderBounds =
      leftSliderBounds.withX(leftSliderBounds.getX() + sliderOffsetX);
    zoomSlider.setBounds(leftSliderBounds);

    auto rightSliderBounds = bounds.removeFromRight(sliderWidth);
    rightSliderBounds =
      rightSliderBounds.withX(rightSliderBounds.getX() - sliderOffsetX);

    thicknessSlider.setBounds(
      rightSliderBounds.removeFromTop(rightSliderBounds.getHeight() / 2));
    heightSlider.setBounds(rightSliderBounds);

    oscilloscopeComponent.setBounds(bounds);
  }
  //============================================================================
  void sliderValueChanged(juce::Slider* slider) override
  {
    if (slider == &zoomSlider.getSlider()) {
      oscilloscopeComponent.setZoom(zoomSlider.getSlider().getValue());
    } else if (slider == &thicknessSlider.getSlider()) {
      oscilloscopeComponent.setThickness(
        thicknessSlider.getSlider().getValue());
    } else if (slider == &heightSlider.getSlider()) {
      oscilloscopeComponent.setHeight(heightSlider.getSlider().getValue());
    }
  }
  //============================================================================
private:
  OscilloscopeComponent oscilloscopeComponent;
  LinearSlider zoomSlider;
  LinearSlider thicknessSlider;
  LinearSlider heightSlider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopePanel)
};

} // namespace panel
} // namespace gui
} // namespace dmt