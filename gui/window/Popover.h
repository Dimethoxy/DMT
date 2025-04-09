//==============================================================================
#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace window {
//==============================================================================
class Popover : public juce::Component
{
  using String = juce::String;
  using Rectangle = juce::Rectangle<int>;

  using Settings = dmt::Settings;
  using Layout = dmt::Settings::Window;

  // Window
  const float& size = dmt::Settings::Window::size;

  // Popover
  const float rawSpikeWidth = 20.0f;
  const float rawSpikeHeight = 20.0f;
  const int rawSurfaceWidth = 200;
  const int rawSurfaceHeight = 100;

public:
  Popover()
  {
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(false, false);
  }
  ~Popover() override { setVisible(false); }

  void paint(juce::Graphics& g) override
  {
    g.fillAll(juce::Colours::green.withAlpha(0.2f));

    // Draw the surface
    const auto surfacePath = getSurfacePath();
    g.setColour(juce::Colours::red);
    g.fillPath(surfacePath);
  }
  void resized() override {}

  void showMessage(Point<int> _anchor)
  {
    this->setNormalizedAnchor(_anchor);
    this->setVisible(true);
    repaint();
  }

protected:
  juce::Path getSurfacePath()
  {
    juce::Path path;

    // Calculate dimensions
    const int spikeWidth = rawSpikeWidth * size;
    const int spikeHeight = rawSpikeHeight * size;
    const int surfaceWidth = rawSurfaceWidth * size;
    const int surfaceHeight = rawSurfaceHeight * size;

    // Configure message bounds
    Rectangle messageBounds;
    const auto anchor = getAnchor();
    messageBounds.setSize(surfaceWidth, surfaceHeight);
    const auto messageBoundsOffsetY = surfaceHeight / 2 + spikeHeight;
    const auto messageBoundsCentreX = anchor.x;
    const auto messageBoundsCentreY = anchor.y + messageBoundsOffsetY;
    messageBounds.setCentre(messageBoundsCentreX, messageBoundsCentreY);

    // Calculate spike points
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

    // Create path
    path.startNewSubPath(spikeBaseLeft);
    path.lineTo(spikeTip);
    path.lineTo(spikeBaseRight);
    path.lineTo(messageBounds.getTopRight().toFloat());
    path.lineTo(messageBounds.getBottomRight().toFloat());
    path.lineTo(messageBounds.getBottomLeft().toFloat());
    path.lineTo(messageBounds.getTopLeft().toFloat());
    path.closeSubPath();
    return path;
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
  std::unique_ptr<juce::Point<float>> normalizedAnchor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Popover)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt