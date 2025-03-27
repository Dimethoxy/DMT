#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingAudioBuffer.h"
#include "gui/component/AbstractDisplayComponent.h"
#include "gui/widget/Oscilloscope.h"
#include "gui/widget/Shadow.h"
#include "utility/RepaintTimer.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
template<typename SampleType>
class OscilloscopeComponent
  : public dmt::gui::component::AbstractDisplayComponent
  , public juce::AudioProcessorValueTreeState::Listener
{
  using Oscilloscope = dmt::gui::widget::Oscilloscope<SampleType>;
  using RingAudioBuffer = dmt::dsp::data::RingAudioBuffer<SampleType>;
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<SampleType>;
  using Shadow = dmt::gui::widget::Shadow;

  // General
  using Settings = dmt::Settings;
  const juce::Colour& backgroundColour =
    Settings::Oscilloscope::backgroundColour;
  // Layout
  const float& size = Settings::Window::size;
  const float& rawCornerSize = Settings::Oscilloscope::cornerSize;
  const float& rawPadding = Settings::Oscilloscope::padding;
  // Border
  const bool& drawBorder = Settings::Oscilloscope::drawBorder;
  const juce::Colour& borderColour = Settings::Oscilloscope::borderColour;
  const float& borderStrength = Settings::Oscilloscope::borderStrength;
  // Shadows
  const bool& drawOuterShadow = Settings::Oscilloscope::drawOuterShadow;
  const bool& drawInnerShadow = Settings::Oscilloscope::drawInnerShadow;
  const juce::Colour& outerShadowColour =
    Settings::Oscilloscope::outerShadowColour;
  const juce::Colour& innerShadowColour =
    Settings::Oscilloscope::innerShadowColour;
  const float& outerShadowRadius = Settings::Oscilloscope::outerShadowRadius;
  const float& innerShadowRadius = Settings::Oscilloscope::innerShadowRadius;

public:
  //==============================================================================
  OscilloscopeComponent(FifoAudioBuffer& _fifoBuffer,
                        AudioProcessorValueTreeState& _apvts)
    : ringBuffer(2, 4096)
    , fifoBuffer(fifoBuffer)
    , leftOscilloscope(ringBuffer, 0)
    , rightOscilloscope(ringBuffer, 1)
  {
    _apvts.addParameterListener("OscilloscopeZoom", this);
    _apvts.addParameterListener("OscilloscopeThickness", this);
    _apvts.addParameterListener("OscilloscopeGain", this);
  }
  //==============================================================================
  void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    auto scopeBounds =
      _displayBounds.withHeight(_displayBounds.getHeight() * 0.92f)
        .withCentre(_displayBounds.getCentre());

    auto leftScopeBounds =
      scopeBounds.removeFromTop(scopeBounds.getHeight() * 0.5f);
    auto rightScopeBounds = scopeBounds;

    leftOscilloscope.setBounds(leftScopeBounds);
    rightOscilloscope.setBounds(rightScopeBounds);
  }
  //==============================================================================
  void paintDisplay(
    juce::Graphics& g,
    const juce::Rectangle<int>& /*_displayBounds*/) const noexcept override
  {
    TRACER("OscilloscopeComponent::paintDisplay");
    g.drawImageAt(leftOscilloscope.getImage(),
                  leftOscilloscope.getBounds().getX(),
                  leftOscilloscope.getBounds().getY());
    g.drawImageAt(rightOscilloscope.getImage(),
                  rightOscilloscope.getBounds().getX(),
                  rightOscilloscope.getBounds().getY());
  }
  //==============================================================================
  void prepareNextFrame() noexcept override
  {
    TRACER("OscilloscopeComponent::prepareNextFrame");
    ringBuffer.write(fifoBuffer);
    ringBuffer.equalizeReadPositions();
    leftOscilloscope.notify();
    rightOscilloscope.notify();
  }
  //==============================================================================
  void setZoom(float _zoom) noexcept
  {
    TRACER("OscilloscopeComponent::setZoom");
    // Just random math with magic numbers to get a nice feeling zoom
    float zoomModifier = (_zoom + 5) / 105.0f;
    float maxSamplesPerPixel = 900.0f;
    float exponentialModifier = pow(zoomModifier, 4.0f);
    float samplesPerPixel = 1.0f + maxSamplesPerPixel * exponentialModifier;
    leftOscilloscope.setRawSamplesPerPixel(samplesPerPixel);
    rightOscilloscope.setRawSamplesPerPixel(samplesPerPixel);
  }
  //==============================================================================
  void setThickness(float _thickness) noexcept
  {
    TRACER("OscilloscopeComponent::setThickness");
    leftOscilloscope.setThickness(_thickness);
    rightOscilloscope.setThickness(_thickness);
  }
  //==============================================================================
  void setHeight(float _height) noexcept
  {
    TRACER("OscilloscopeComponent::setHeight");
    float amplitude = juce::Decibels::decibelsToGain(_height);
    leftOscilloscope.setAmplitude(amplitude);
    rightOscilloscope.setAmplitude(amplitude);
  }
  //==============================================================================
  virtual void parameterChanged(const String& _parameterID, float _newValue)
  {
    if (_parameterID == "OscilloscopeZoom") {
      setZoom(_newValue);
    }
    if (_parameterID == "OscilloscopeThickness") {
      setThickness(_newValue);
    }
    if (_parameterID == "OscilloscopeGain") {
      setHeight(_newValue);
    }
  }

private:
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Oscilloscope leftOscilloscope;
  Oscilloscope rightOscilloscope;
  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt