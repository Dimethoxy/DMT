//==============================================================================
#pragma once
//==============================================================================
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
    : outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    this->startRepaintTimer();
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
  }
  //============================================================================
  void paint(juce::Graphics& g) override final
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
    // Draw display
    paintDisplay(g, innerBounds);

    // We need to draw the border again because drawing it once didn't cut it
    if (drawBorder) {
      g.setColour(borderColour);
      const auto borderBounds = outerBounds.reduced(borderStrength / 2.0f);
      g.drawRoundedRectangle(
        borderBounds.toFloat(), outerCornerSize, borderStrength);
    }

    // Prepare next frame
    prepareNextFrame();
  }
  //============================================================================
  void resized() override final
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

    // Call the childs extendResized method
    extendResized(drawBorder ? innerBounds : outerBounds);
  }
  //============================================================================
protected:
  //============================================================================
  virtual void extendResized(
    const juce::Rectangle<int>& displayBounds) noexcept = 0;
  //============================================================================
  /**
   * @brief Repaints the display.
   *
   * @note Do the actual drawing in this method.
   * @warning Do not call this method directly.
   *
   * @param g Graphics context used for drawing.
   * @param displayBounds Bounds of the display area.
   */
  virtual void paintDisplay(
    juce::Graphics& g,
    const juce::Rectangle<int>& displayBounds) const noexcept = 0;
  //============================================================================
  /**
   * @brief Prepares the next frame.
   *
   * @warning Do not draw anything or do any heavy calculations in this method.
   *          Drawing should be done on another thread.
   *          We really don't want to block this thread.
   */
  virtual void prepareNextFrame() noexcept = 0;
  //============================================================================
private:
  //============================================================================
  void repaintTimerCallback() noexcept override final { this->repaint(); }
  //============================================================================
private:
  juce::Rectangle<int> innerBounds;
  juce::Rectangle<int> outerBounds;
  Shadow outerShadow =
    Shadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false);
  Shadow innerShadow =
    Shadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true);
};
} // namespace components
} // namespace gui
} // namespace dmt