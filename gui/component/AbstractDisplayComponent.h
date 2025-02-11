//==============================================================================

#pragma once

#include "gui/widget/Shadow.h"
#include "utility/RepaintTimer.h"
#include "utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class AbstractDisplayComponent
  : public juce::Component
  , public dmt::utility::RepaintTimer
{
  using Shadow = dmt::gui::widget::Shadow;

  // General
  using Display = dmt::Settings::Display;
  const juce::Colour& backgroundColour = Display::backgroundColour;
  // Layout
  const float& size = Settings::Window::size;
  const float& rawCornerSize = Display::cornerSize;
  const float& rawPadding = Display::padding;
  // Border
  const bool& drawBorder = Display::drawBorder;
  const juce::Colour& borderColour = Display::borderColour;
  const float& rawBorderStrength = Display::borderStrength;
  // Shadows
  const bool& drawOuterShadow = Display::drawOuterShadow;
  const bool& drawInnerShadow = Display::drawInnerShadow;
  const juce::Colour& outerShadowColour = Display::outerShadowColour;
  const juce::Colour& innerShadowColour = Display::innerShadowColour;
  const float& outerShadowRadius = Display::outerShadowRadius;
  const float& innerShadowRadius = Display::innerShadowRadius;

public:
  //============================================================================
  AbstractDisplayComponent(/*juce::AudioProcessorValueTreeState& apvts*/)
    : // apvts(apvts),
    outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    this->startRepaintTimer();
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    // Precalculation
    const auto borderStrength = rawBorderStrength * size;
    const auto cornerSize = rawCornerSize * size;
    const float outerCornerSize = cornerSize;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * 0.5f), 0.0f, outerCornerSize);

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
  }
  //============================================================================
  void resized() override
  {
    TRACER("DisfluxDisplayComponent::resized");

    const auto bounds = getLocalBounds();
    const auto borderStrength = rawBorderStrength * size;
    const auto cornerSize = rawCornerSize * size;
    const auto padding = rawPadding * size;

    outerBounds = bounds.reduced(padding);
    innerBounds = outerBounds.reduced(borderStrength);
    const float outerCornerSize = cornerSize;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * 0.5f), 0.0f, outerCornerSize);

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
  }
  //============================================================================
  void repaintTimerCallback() noexcept override { this->repaint(); }
  //============================================================================
  virtual void repaintDisplay(
    juce::Graphics& g,
    const juce::Rectangle<int>& displayBounds) const noexcept = 0;

private:
  juce::Rectangle<int> innerBounds;
  juce::Rectangle<int> outerBounds;
  Shadow outerShadow =
    Shadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false);
  Shadow innerShadow =
    Shadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true);

  // juce::AudioProcessorValueTreeState& apvts;
};
} // namespace components
} // namespace gui
} // namespace dmt