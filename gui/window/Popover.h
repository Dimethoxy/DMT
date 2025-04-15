//==============================================================================
#pragma once
//==============================================================================
#include "gui/widget/CallbackButton.h"
#include "utility/Math.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace window {
//==============================================================================
class Popover : public juce::Component
{
  using CallbackButton = dmt::gui::widget::CallbackButton;
  using String = juce::String;
  using Rectangle = juce::Rectangle<int>;
  using Colour = juce::Colour;
  using Shadow = dmt::gui::widget::Shadow;

  using Settings = dmt::Settings;
  using Layout = dmt::Settings::Window;
  using PopoverSettings = Settings::Popover;

  // Window
  const float& size = dmt::Settings::Window::size;

  // Popover
  const Colour backgroundColour = PopoverSettings::backgroundColour;
  const Colour borderColour = PopoverSettings::borderColour;
  const Colour textColour = PopoverSettings::textColour;
  const Colour innerShadowColour = PopoverSettings::innerShadowColour;
  const Colour outerShadowColour = PopoverSettings::outerShadowColour;
  const int rawSurfaceWidth = PopoverSettings::rawSurfaceWidth;
  const int rawSurfaceHeight = PopoverSettings::rawSurfaceHeight;
  const int rawCornerRadius = PopoverSettings::rawCornerRadius;
  const int rawBorderWidth = PopoverSettings::rawBorderWidth;
  const float innerShadowRadius = PopoverSettings::innerShadowRadius;
  const float outerShadowRadius = PopoverSettings::outerShadowRadius;
  const bool drawOuterShadow = PopoverSettings::drawOuterShadow;
  const bool drawInnerShadow = PopoverSettings::drawInnerShadow;
  const float rawSpikeWidth = 20.0f;
  const float rawSpikeHeight = 20.0f;
  const int rawCloseButtonSize = 35;

public:
  Popover()
    : outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(false, true);

    addAndMakeVisible(closeButton);
    closeButton.onClick = [this] { hideMessage(); };

    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
  }
  ~Popover() override { setVisible(false); }

  void paint(juce::Graphics& g) override
  {
    // Skip drawing if the anchor is null
    if (normalizedAnchor == nullptr)
      return;

    // Draw the border
    g.setColour(borderColour);
    g.fillPath(createPath(true));

    // Draw the background
    g.setColour(backgroundColour);
    g.fillPath(createPath(false));
  }

  void resized() override
  {
    auto messageBounds = createMessageBounds(false);
    const auto closeButtonSize = rawCloseButtonSize * size;
    closeButton.setBounds(messageBounds.removeFromTop(closeButtonSize)
                            .removeFromRight(closeButtonSize));
    closeButton.setAlwaysOnTop(true);

    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    outerShadow.setPath(createPath(true));
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    innerShadow.setPath(createPath(false));
  }

  void showMessage(Point<int> _anchor)
  {
    this->setNormalizedAnchor(_anchor);
    this->setVisible(true);
    resized();
    repaint();
  }

  void hideMessage()
  {
    normalizedAnchor.reset();
    this->setVisible(false);
  }

protected:
  juce::Path createPath(bool isOuter = true)
  {
    juce::Path path;

    const auto messageBounds = createMessageBounds(isOuter);
    const auto spikePoints = calculateSpikePoints(isOuter);

    addSpikeToPath(path, spikePoints);
    addRoundedRectangleToPath(path, messageBounds, isOuter);

    path.closeSubPath();
    return path;
  }

  Rectangle createMessageBounds(bool isOuter) const
  {
    const int surfaceWidth = rawSurfaceWidth * size;
    const int surfaceHeight = rawSurfaceHeight * size;
    const float borderWidth = isOuter ? 0.0f : rawBorderWidth * size;

    Rectangle messageBounds;
    const auto anchor = getAnchor();
    messageBounds.setSize(surfaceWidth, surfaceHeight);
    const auto messageBoundsOffsetY = surfaceHeight / 2 + rawSpikeHeight * size;
    const auto messageBoundsCentreX = anchor.x;
    const auto messageBoundsCentreY = anchor.y + messageBoundsOffsetY;
    messageBounds.setCentre(messageBoundsCentreX, messageBoundsCentreY);

    if (!isOuter)
      messageBounds = messageBounds.reduced(borderWidth);

    return messageBounds;
  }

  std::tuple<juce::Point<float>, juce::Point<float>, juce::Point<float>>
  calculateSpikePoints(bool isOuter) const
  {
    const int spikeWidth = rawSpikeWidth * size;
    const int spikeHeight = rawSpikeHeight * size;
    const float borderWidth = isOuter ? 0.0f : rawBorderWidth * size;

    const auto anchor = getAnchor();
    const auto spikeTipX = anchor.x;
    const auto spikeTipY = anchor.y;
    const auto spikeBaseLeftX = anchor.x - spikeWidth / 2;
    const auto spikeBaseLeftY = anchor.y + spikeHeight;
    const auto spikeBaseRightX = anchor.x + spikeWidth / 2;
    const auto spikeBaseRightY = anchor.y + spikeHeight;

    const auto spikeTip = juce::Point<float>(spikeTipX, spikeTipY);
    const auto spikeBaseLeft =
      juce::Point<float>(spikeBaseLeftX, spikeBaseLeftY);
    const auto spikeBaseRight =
      juce::Point<float>(spikeBaseRightX, spikeBaseRightY);

    if (isOuter)
      return { spikeTip, spikeBaseLeft, spikeBaseRight };

    const auto leftSlope =
      dmt::math::calculateSlope<float, float>(spikeBaseLeft, spikeTip)
        .value_or(0.0f);
    const auto rightSlope =
      dmt::math::calculateSlope<float, float>(spikeBaseRight, spikeTip)
        .value_or(0.0f);

    const auto leftAngle = dmt::math::slopeToAngleInRadians<float>(leftSlope);
    const auto rightAngle = dmt::math::slopeToAngleInRadians<float>(rightSlope);

    const auto rotatedLeftAngle =
      leftAngle + juce::MathConstants<float>::halfPi;
    const auto rotatedRightAngle =
      rightAngle + juce::MathConstants<float>::halfPi;

    const auto marchedLeft =
      dmt::math::marchPoint(spikeBaseLeft, rotatedLeftAngle, borderWidth);
    const auto marchedRight =
      dmt::math::marchPoint(spikeBaseRight, rotatedRightAngle, borderWidth);

    const auto leftBaseProjection =
      dmt::math::projectPointToY(
        marchedLeft, leftAngle, spikeBaseLeftY + borderWidth)
        .value();

    const auto rightBaseProjection =
      dmt::math::projectPointToY(
        marchedRight, rightAngle, spikeBaseRightY + borderWidth)
        .value();

    const auto intersection =
      dmt::math::intersectInfiniteLines<float>(
        leftBaseProjection, marchedLeft, rightBaseProjection, marchedRight)
        .value();

    return { intersection, leftBaseProjection, rightBaseProjection };
  }

  void addSpikeToPath(juce::Path& path,
                      const std::tuple<juce::Point<float>,
                                       juce::Point<float>,
                                       juce::Point<float>>& spikePoints) const
  {
    const auto& [spikeTip, spikeBaseLeft, spikeBaseRight] = spikePoints;

    path.startNewSubPath(spikeBaseLeft);
    path.lineTo(spikeTip);
    path.lineTo(spikeBaseRight);
  }

  void addRoundedRectangleToPath(juce::Path& path,
                                 const Rectangle& messageBounds,
                                 bool isOuter) const
  {
    const auto topLeft = messageBounds.getTopLeft().toFloat();
    const auto topRight = messageBounds.getTopRight().toFloat();
    const auto bottomLeft = messageBounds.getBottomLeft().toFloat();
    const auto bottomRight = messageBounds.getBottomRight().toFloat();

    const auto cornerRadius =
      (rawCornerRadius - (isOuter ? 0.0f : rawBorderWidth)) * size;

    // Top flat edge
    path.lineTo(topRight.x - cornerRadius, topRight.y);

    // Top right corner
    path.addArc(topRight.x - cornerRadius,
                topRight.y,
                cornerRadius,
                cornerRadius,
                0.0f,
                juce::MathConstants<float>::halfPi);

    // Right flat edge
    path.lineTo(bottomRight.x, bottomRight.y - cornerRadius);

    // Bottom right corner
    path.addArc(bottomRight.x - cornerRadius,
                bottomRight.y - cornerRadius,
                cornerRadius,
                cornerRadius,
                juce::MathConstants<float>::halfPi,
                juce::MathConstants<float>::halfPi * 2.0f);

    // Bottom flat edge
    path.lineTo(bottomLeft.x + cornerRadius, bottomLeft.y);

    // Bottom left corner
    path.addArc(bottomLeft.x,
                bottomLeft.y - cornerRadius,
                cornerRadius,
                cornerRadius,
                juce::MathConstants<float>::halfPi * 2.0f,
                juce::MathConstants<float>::halfPi * 3.0f);

    // Left flat edge
    path.lineTo(topLeft.x, topLeft.y + cornerRadius);

    // Top left corner
    path.addArc(topLeft.x,
                topLeft.y,
                cornerRadius,
                cornerRadius,
                juce::MathConstants<float>::halfPi * 3.0f,
                juce::MathConstants<float>::halfPi * 4.0f);
  }

  void setNormalizedAnchor(const juce::Point<int>& _anchor)
  {
    if (getWidth() == 0 || getHeight() == 0)
      return;

    const auto normalizedX = static_cast<float>(_anchor.x) / getWidth();
    const auto normalizedY = static_cast<float>(_anchor.y) / getHeight();
    normalizedAnchor =
      std::make_unique<juce::Point<float>>(normalizedX, normalizedY);
  }

  juce::Point<int> getAnchor() const
  {
    if (normalizedAnchor == nullptr)
      return juce::Point<int>(0, 0);

    const auto denormalizedX =
      static_cast<int>(normalizedAnchor->x * getWidth());
    const auto denormalizedY =
      static_cast<int>(normalizedAnchor->y * getHeight());
    return juce::Point<int>(denormalizedX, denormalizedY);
  }

private:
  CallbackButton closeButton{ "CloseButton", "Close", "Close", false,
                              false,         false,   true };
  Shadow outerShadow;
  Shadow innerShadow;

  std::unique_ptr<juce::Point<float>> normalizedAnchor;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Popover)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt