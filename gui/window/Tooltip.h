#pragma once

//==============================================================================

#include <JuceHeader.h>
#include "dmt/utility/RepaintTimer.h"

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

class Tooltip : public juce::Component, private dmt::utility::RepaintTimer
{

  const float& rawFontSize = 18.0f;

public:
  Tooltip() noexcept
  {
    setInterceptsMouseClicks(false, false);
    startRepaintTimer(); // Use custom repaint timer
  }

  ~Tooltip() override = default;

  void resized() override {}

  void paint(juce::Graphics& graphics) override
  {
    if (!tooltipImage.isNull())
    {
      const int imageWidth = tooltipImage.getWidth();
      const int imageHeight = tooltipImage.getHeight();
      const int width = getWidth();
      const int height = getHeight();

      int drawPositionX = lastMousePosition.x;
      int drawPositionY = lastMousePosition.y;

      // If drawing at (x, y) would go off the right edge, flip to left of mouse
      if (drawPositionX + imageWidth > width)
        drawPositionX = std::max(0, lastMousePosition.x - imageWidth);

      // If drawing at (x, y) would go off the bottom, flip to above mouse
      if (drawPositionY + imageHeight > height)
        drawPositionY = std::max(0, lastMousePosition.y - imageHeight);

      graphics.drawImageAt(tooltipImage, drawPositionX, drawPositionY);
    }
  }

  void repaintTimerCallback() noexcept override
  {
    auto* parentComponent = getParentComponent();
    if (!parentComponent)
      return;

    auto mousePosition = parentComponent->getMouseXYRelative();
    auto* component = parentComponent->getComponentAt(mousePosition);

    juce::String foundTooltipText;

    while (component != nullptr && component != parentComponent)
    {
      if (auto* tooltipClient = dynamic_cast<juce::TooltipClient*>(component))
      {
        auto tooltipText = tooltipClient->getTooltip();
        if (!tooltipText.isEmpty())
        {
          foundTooltipText = tooltipText;
          break;
        }
      }
      component = component->getParentComponent();
    }

    bool needsRepaint = false;

    if (foundTooltipText != currentTooltipText)
    {
      currentTooltipText = foundTooltipText;
      if (currentTooltipText.isNotEmpty())
        renderTooltipImage(currentTooltipText);
      else
        tooltipImage = juce::Image(); // Null image
      needsRepaint = true;
    }

    if (mousePosition != lastMousePosition)
    {
      lastMousePosition = mousePosition;
      needsRepaint = true;
    }

    if (needsRepaint)
      repaint();
  }

protected:
  void renderTooltipImage(const juce::String& text)
  {
    // For now, just render a simple white rectangle with the text in black
    constexpr int imageWidth = 300, imageHeight = 40;
    tooltipImage = juce::Image(juce::Image::ARGB, imageWidth, imageHeight, true);
    juce::Graphics graphics(tooltipImage);
    graphics.fillAll(juce::Colours::white);
    graphics.setColour(juce::Colours::black);
    graphics.setFont(rawFontSize);
    graphics.drawText(text, tooltipImage.getBounds(), juce::Justification::centredLeft, true);
  }

private:
  juce::String currentTooltipText;
  juce::Image tooltipImage;
  juce::Point<int> lastMousePosition;

};

} // namespace window
} // namespace gui
} // namespace dmt