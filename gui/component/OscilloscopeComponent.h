#pragma once
//==============================================================================
#include "dsp/data/FifoAudioBuffer.h"
#include "dsp/data/RingAudioBuffer.h"
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
  : public juce::Component
  , public dmt::utility::RepaintTimer
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
  OscilloscopeComponent(FifoAudioBuffer& fifoBuffer)
    : ringBuffer(2, 4096)
    , fifoBuffer(fifoBuffer)
    , leftOscilloscope(ringBuffer, 0)
    , rightOscilloscope(ringBuffer, 1)
  {
    startRepaintTimer();
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
  }
  //==============================================================================
  void repaintTimerCallback() noexcept override
  {
    TRACER("OscilloscopeComponent::repaintTimerCallback");
    repaint();
    ringBuffer.write(fifoBuffer);
    ringBuffer.equalizeReadPositions();
    leftOscilloscope.notify();
    rightOscilloscope.notify();
  }
  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    TRACER("OscilloscopeComponent::paint");
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // Draw background if border is disabled
    if (!drawBorder) {
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(outerBounds.toFloat(), outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(borderColour);
      g.fillRoundedRectangle(outerBounds.toFloat(), outerCornerSize);
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(innerBounds.toFloat(), innerCornerSize);
    }

    // Paint Oscilloscope Vertical Lines
    const auto leftScopeBounds = leftOscilloscope.getBounds().toFloat();
    const auto rightScopeBounds = rightOscilloscope.getBounds().toFloat();
    const int scopeX = leftScopeBounds.getX();
    const float scopeWidth = leftScopeBounds.getWidth();
    const float leftScopeY = leftScopeBounds.getY();
    const float leftScopeHeight = leftScopeBounds.getHeight();
    const float rightScopeY = rightScopeBounds.getY();
    const float rightScopeHeight = rightScopeBounds.getHeight();
    g.setColour(backgroundColour.brighter(0.05));
    const int numLines = getWidth() / (getHeight() / 4.0f);
    const float lineSpacing = scopeWidth / numLines;
    for (size_t i = 1; i < numLines; ++i) {
      const float x = scopeX + lineSpacing * i;
      g.drawLine(
        juce::Line<float>(x, leftScopeY, x, leftScopeY + leftScopeHeight),
        2.0f * size);
      g.drawLine(
        juce::Line<float>(x, rightScopeY, x, rightScopeY + rightScopeHeight),
        2.0f * size);
    }
    float lineThicknessModifiers[7] = {
      1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f
    };
    float brightnessValues[7] = { 0.15f, 0.05f, 0.05f, 0.05f,
                                  0.05f, 0.05f, 0.15f };
    for (size_t i = 0; i < 7; ++i) {
      const float y = leftScopeY + (leftScopeHeight / 6) * i;
      g.setColour(backgroundColour.brighter(brightnessValues[i]));
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i] * size);
    }
    for (size_t i = 0; i < 7; ++i) {
      const float y = rightScopeY + (rightScopeHeight / 6) * i;
      g.setColour(backgroundColour.brighter(brightnessValues[i]));
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i] * size);
    }

    g.drawImageAt(leftOscilloscope.getImage(),
                  leftOscilloscope.getBounds().getX(),
                  leftOscilloscope.getBounds().getY());
    g.drawImageAt(rightOscilloscope.getImage(),
                  rightOscilloscope.getBounds().getX(),
                  rightOscilloscope.getBounds().getY());
  }
  //==============================================================================
  void resized() override
  {
    TRACER("OscilloscopeComponent::resized");
    auto bounds = getLocalBounds();
    outerBounds = bounds.reduced(rawPadding * size);
    innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    juce::Path outerShadowPath;
    outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
    outerShadow.setPath(outerShadowPath);
    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    outerShadow.toBack();

    juce::Path innerShadowPath;
    innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
    innerShadow.setPath(innerShadowPath);
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    innerShadow.toBack();

    auto rawScopeBounds = outerBounds;
    if (drawBorder) {
      rawScopeBounds = innerBounds;
    }
    auto scopeBounds =
      rawScopeBounds.withHeight(rawScopeBounds.getHeight() * 0.92f)
        .withCentre(bounds.getCentre());

    auto leftScopeBounds =
      scopeBounds.removeFromTop(scopeBounds.getHeight() * 0.5f);
    auto rightScopeBounds = scopeBounds;

    leftOscilloscope.setBounds(leftScopeBounds);
    rightOscilloscope.setBounds(rightScopeBounds);
  }
  //==============================================================================
  void setZoom(float zoom) noexcept
  {
    TRACER("OscilloscopeComponent::setZoom");
    // Just random math with magic numbers to get a nice feeling zoom
    float zoomModifier = (zoom + 5) / 105.0f;
    float maxSamplesPerPixel = 900.0f;
    float exponentialModifier = pow(zoomModifier, 4.0f);
    float samplesPerPixel = 1.0f + maxSamplesPerPixel * exponentialModifier;
    leftOscilloscope.setRawSamplesPerPixel(samplesPerPixel);
    rightOscilloscope.setRawSamplesPerPixel(samplesPerPixel);
  }
  //==============================================================================
  void setThickness(float thickness) noexcept
  {
    TRACER("OscilloscopeComponent::setThickness");
    leftOscilloscope.setThickness(thickness);
    rightOscilloscope.setThickness(thickness);
  }
  //==============================================================================
  void setHeight(float height) noexcept
  {
    TRACER("OscilloscopeComponent::setHeight");
    float amplitude = juce::Decibels::decibelsToGain(height);
    leftOscilloscope.setAmplitude(amplitude);
    rightOscilloscope.setAmplitude(amplitude);
  }
  //==============================================================================
private:
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Oscilloscope leftOscilloscope;
  Oscilloscope rightOscilloscope;
  //==============================================================================
  juce::Rectangle<int> backgroundBounds;
  Shadow outerShadow =
    Shadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false);
  Shadow innerShadow =
    Shadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true);
  juce::Rectangle<int> innerBounds;
  juce::Rectangle<int> outerBounds;
  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt