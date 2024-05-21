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
  {
    addAndMakeVisible(leftOscilloscope);
    addAndMakeVisible(rightOscilloscope);
    startRepaintTimer();
  }

  //==============================================================================
  void paint(juce::Graphics& g) override
  {
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(rawPadding * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
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
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(borderColour);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(innerBounds, innerCornerSize);
    }

    // Paint Oscilloscope Horizontal Lines
    const float scopeX = leftOscilloscope.getX();
    const float scopeWidth = leftOscilloscope.getWidth();
    const float leftScopeY = leftOscilloscope.getY();
    const float leftScopeHeight = leftOscilloscope.getHeight();
    g.setColour(Settings::Colours::background.brighter(0.05));
    float lineThicknessModifiers[5] = { 1.5f, 1.0f, 1.5f, 1.0f, 1.5f };
    for (int i = 0; i < 5; ++i) {
      const float y = leftScopeY + (leftScopeHeight * 0.25f) * i;
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i]);
    }
    const float rightScopeY = rightOscilloscope.getY();
    const float rightScopeHeight = rightOscilloscope.getHeight();
    for (int i = 0; i < 5; ++i) {
      const float y = rightScopeY + (rightScopeHeight * 0.25f) * i;
      g.drawLine(juce::Line<float>(scopeX, y, scopeX + scopeWidth, y),
                 3.0f * lineThicknessModifiers[i]);
    }

    // Paint Oscilloscope Vertical Lines
    const int numLines = 20;
    const float lineSpacing = scopeWidth / numLines;
    for (int i = 0; i < numLines; ++i) {
      const float x = scopeX + lineSpacing * i;
      g.drawLine(
        juce::Line<float>(x, leftScopeY, x, leftScopeY + leftScopeHeight),
        3.0f);
      g.drawLine(
        juce::Line<float>(x, rightScopeY, x, rightScopeY + rightScopeHeight),
        3.0f);
    }

    // Draw the inner shadow
    if (drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }

    leftOscilloscope.repaint();
    rightOscilloscope.repaint();
  }
  //==============================================================================
  void repaintTimerCallback() noexcept override
  {
    ringBuffer.write(fifoBuffer);
    ringBuffer.equalizeReadPositions();
    leftOscilloscope.prepareToPaint();
    rightOscilloscope.prepareToPaint();
    this->repaint();
  }
  //==============================================================================
  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto outerBounds = bounds.reduced(rawPadding * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    auto rawScopeBounds = outerBounds;
    if (drawBorder) {
      rawScopeBounds = innerBounds;
    }
    auto scopeBounds =
      rawScopeBounds.withHeight(rawScopeBounds.getHeight() * 0.9f)
        .withCentre(bounds.getCentre());

    auto leftScopeBounds =
      scopeBounds.removeFromTop(scopeBounds.getHeight() * 0.5f);
    auto rightScopeBounds = scopeBounds;

    leftOscilloscope.setBounds(
      leftScopeBounds.removeFromTop(leftScopeBounds.getHeight() * 0.98));
    rightOscilloscope.setBounds(
      rightScopeBounds.removeFromBottom(rightScopeBounds.getHeight() * 0.98));
  }
  //==============================================================================
private:
  RingAudioBuffer ringBuffer;
  FifoAudioBuffer& fifoBuffer;
  Oscilloscope leftOscilloscope;
  Oscilloscope rightOscilloscope;
  juce::Rectangle<int> backgroundBounds;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt