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
  const float& size = Settings::Layout::size;
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
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
    , leftThread(&OscilloscopeComponent::leftWorker, this)
    , rightThread(&OscilloscopeComponent::rightWorker, this)
  {
    addAndMakeVisible(leftOscilloscope);
    addAndMakeVisible(rightOscilloscope);
    startRepaintTimer();

    leftThread.detach();
    rightThread.detach();
  }
  ~OscilloscopeComponent() override
  {
    exit = true;

    std::lock_guard<std::mutex> leftLock(leftExitMutex);
    leftCondition.notify_one();
    leftExitCondition.wait(leftLock,
                           [this] { return leftThreadExited == true; });

    std::lock_guard<std::mutex> rightLock(rightExitMutex);
    rightCondition.notify_one();
    rightExitCondition.wait(rightLock,
                            [this] { return rightThreadExited == true; });
  }
  //==============================================================================
  void leftWorker() noexcept
  {
    std::unique_lock<std::mutex> lock(leftMutex);
    while (!exit) {
      leftCondition.wait(lock, [this] { return leftPaintFlag || exit; });
      if (leftPaintFlag) {
        leftOscilloscope.render();
        leftPaintFlag = false;
      }
    }
    leftThreadExited = true;
    leftExitCondition.notify_one();
  }
  void rightWorker() noexcept
  {
    std::unique_lock<std::mutex> lock(rightMutex);
    while (!exit) {
      rightCondition.wait(lock, [this] { return rightPaintFlag || exit; });
      if (rightPaintFlag) {
        rightOscilloscope.render();
        rightPaintFlag = false;
      }
    }
    rightThreadExited = true;
    rightExitCondition.notify_one();
  }
  void repaintTimerCallback() noexcept override
  {
    TRACE_COMPONENT();
    ringBuffer.write(fifoBuffer);
    ringBuffer.equalizeReadPositions();
    if (!leftPaintFlag && !rightPaintFlag) {
      rightOscilloscope.repaint();
      leftOscilloscope.repaint();
      leftPaintFlag = true;
      rightPaintFlag = true;
      leftCondition.notify_one();
      rightCondition.notify_one();
    }
  }
  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    TRACE_COMPONENT();

    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // Draw outer shadow
    if (drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

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
    const int scopeX = leftOscilloscope.getX();
    const float scopeWidth = leftOscilloscope.getWidth();
    const float leftScopeY = leftOscilloscope.getY();
    const float leftScopeHeight = leftOscilloscope.getHeight();
    const float rightScopeY = rightOscilloscope.getY();
    const float rightScopeHeight = rightOscilloscope.getHeight();

    g.setColour(Settings::Colours::background.brighter(0.05));
    const int numLines = getWidth() / (getHeight() / 4.0f);
    const float lineSpacing = scopeWidth / numLines;
    for (int i = 0; i < numLines; ++i) {
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
    for (int i = 0; i < 7; ++i) {
      const float y = leftScopeY + (leftScopeHeight / 6) * i;
      g.setColour(Settings::Colours::background.brighter(brightnessValues[i]));
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i] * size);
    }

    for (int i = 0; i < 7; ++i) {
      const float y = rightScopeY + (rightScopeHeight / 6) * i;
      g.setColour(Settings::Colours::background.brighter(brightnessValues[i]));
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i] * size);
    }

    // Draw the inner shadow
    if (drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }
  //==============================================================================
  void resized() override
  {
    auto bounds = getLocalBounds();
    outerBounds = bounds.reduced(rawPadding * size);
    innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

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

    leftOscilloscope.setBounds(
      leftScopeBounds.removeFromTop(leftScopeBounds.getHeight() * 0.95));
    rightOscilloscope.setBounds(
      rightScopeBounds.removeFromBottom(rightScopeBounds.getHeight() * 0.95));
  }
  //==============================================================================
  void setZoom(float zoom) noexcept
  {
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
    leftOscilloscope.setThickness(thickness);
    rightOscilloscope.setThickness(thickness);
  }
  //==============================================================================
  void setHeight(float height) noexcept
  {
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
  Shadow outerShadow;
  Shadow innerShadow;
  juce::Rectangle<int> innerBounds;
  juce::Rectangle<int> outerBounds;
  //==============================================================================
  std::thread leftThread;
  std::thread rightThread;
  std::mutex leftMutex;
  std::mutex rightMutex;
  std::condition_variable leftCondition;
  std::condition_variable rightCondition;
  std::atomic<bool> leftPaintFlag = false;
  std::atomic<bool> rightPaintFlag = false;
  //==============================================================================
  std::atomic<bool> exit = false;
  std::mutex leftExitMutex;
  std::mutex rightExitMutex;
  std::condition_variable leftExitCondition;
  std::condition_variable rightExitCondition;
  std::atomic<bool> leftThreadExited = false;
  std::atomic<bool> rightThreadExited = false;
  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt