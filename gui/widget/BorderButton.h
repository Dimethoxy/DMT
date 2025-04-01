#pragma once
//==============================================================================
#include "utility/RepaintTimer.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class BorderButton
  : public juce::Button
  , private dmt::utility::RepaintTimer
{
  using Settings = dmt::Settings;
  using Colour = juce::Colour;
  using Graphics = juce::Graphics;
  using Image = juce::Image;
  using Rectangle = juce::Rectangle<int>;

  // General
  const float& size = Settings::Window::size;

  // Constants for opacity and raw fade speed
  static constexpr float MAX_OPACITY = 1.0f;    // Fully visible
  static constexpr float MIN_OPACITY = 0.0f;    // Semi-transparent
  static constexpr float RAW_FADE_SPEED = 0.8f; // Base fade speed (per second)

public:
  using ButtonCallback = std::function<void()>;

  BorderButton()
    : juce::Button("BorderButton")
    , currentOpacity(MIN_OPACITY) // Start with semi-transparency
    , isHovered(false)
  {
    addMouseListener(this, true);
    startRepaintTimer(); // Start the timer in the constructor
  }

  ~BorderButton() override {}

  void resized() override
  {
    // Update the cached image when resized
    updateCachedImage();
  }

  void paintButton(Graphics& g,
                   bool /*isMouseOverButton*/,
                   bool /*isButtonDown*/) override
  {
    // Draw the cached image with the current opacity
    g.setOpacity(currentOpacity);
    g.drawImageAt(cachedImage, 0, 0);
  }

  void mouseEnter(const juce::MouseEvent&) override
  {
    isHovered = true;             // Set hover state to true
    currentOpacity = MAX_OPACITY; // Instantly set to max opacity
    repaint(); // Trigger a repaint to reflect the change immediately
  }

  void mouseExit(const juce::MouseEvent&) override
  {
    isHovered = false; // Set hover state to false
  }

  void setOpacityToMax()
  {
    currentOpacity = MAX_OPACITY;
    repaint(); // Trigger a repaint to reflect the change immediately
  }

  void setButtonCallback(ButtonCallback callback)
  {
    buttonCallback = std::move(callback);
  }

  void clicked() override
  {
    if (buttonCallback) {
      buttonCallback(); // Invoke the callback when the button is clicked
    }
  }

private:
  void updateCachedImage()
  {
    // Create or update the cached image
    cachedImage = Image(juce::Image::ARGB, getWidth(), getHeight(), true);
    Graphics g(cachedImage);

    // Draw the image content
    g.fillAll(juce::Colours::transparentBlack); // Clear the image
    g.setColour(juce::Colours::green);
    g.fillRoundedRectangle(Rectangle(0, 0, getWidth(), getHeight()).toFloat(),
                           5.0f);

    // Add additional drawing logic here if needed
    g.setColour(juce::Colours::white);
    g.drawText(
      "Click to show Header", getLocalBounds(), juce::Justification::centred);
  }

  void repaintTimerCallback() noexcept override
  {
    // Calculate fade speed dynamically based on frame rate
    const float fadeSpeed =
      RAW_FADE_SPEED / static_cast<float>(Settings::framerate);

    // Adjust opacity based on hover state
    if (!isHovered) { // Only fade out when not hovered
      currentOpacity =
        std::max(MIN_OPACITY, currentOpacity - fadeSpeed); // Fade out
      repaint(); // Trigger a repaint to update the button
    }
  }

  Image cachedImage;             // Cached image for optimized rendering
  float currentOpacity;          // Current transparency level
  bool isHovered;                // Whether the button is hovered
  ButtonCallback buttonCallback; // Callback to be invoked on click
};
} // namespace widget
} // namespace dmt
} // namespace gui