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
  const float rawTipWidth = 20.0f;
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
    const int tipWidth = rawTipWidth * size;
    const int surfaceWidth = rawSurfaceWidth * size;
    const int surfaceHeight = rawSurfaceHeight * size;

    // Configure message bounds
    Rectangle messageBounds;
    const auto anchor = getAnchor();
    messageBounds.setSize(surfaceWidth, surfaceHeight);
    messageBounds.setCentre(anchor);
    messageBounds.setTop(anchor.y + tipWidth);

    // Create path
    path.addRectangle(messageBounds);

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