#pragma once

//==============================================================================

#include <JuceHeader.h>
#include "dmt/utility/RepaintTimer.h"
#include "dmt/utility/Settings.h"
#include "dmt/utility/Fonts.h"

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

class Tooltip : public juce::Component, private dmt::utility::RepaintTimer
{
  using Settings = dmt::Settings;
  using Fonts = dmt::utility::Fonts;
  //==============================================================================
  // Window 
  const float& size = Settings::Window::size;

  // Tooltip
  const float rawFontSize = 18.0f;
  const Colour fontColour = juce::Colours::white;

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
    const auto fontSize = rawFontSize * size;
    const auto font = fonts.medium.withHeight(fontSize);

    juce::AttributedString attributedString(text);
    attributedString.setFont(font);
    attributedString.setColour(fontColour);
    attributedString.setJustification(juce::Justification::centredLeft);
    attributedString.setWordWrap(juce::AttributedString::WordWrap::none);

    const float maxWidth = getWidth();
    juce::TextLayout textLayout;
    textLayout.createLayout(attributedString, maxWidth);
    
    const auto layoutWidth = textLayout.getWidth() * 1.01f;
    const auto layoutHeight = textLayout.getHeight() * 1.01f;
    tooltipImage = juce::Image(juce::Image::ARGB, layoutWidth, layoutHeight, true);
    juce::Graphics graphics(tooltipImage);
    graphics.setColour(juce::Colours::black);
    graphics.fillAll(juce::Colours::black);
    graphics.setFont(font);
    graphics.setColour(fontColour);
    graphics.drawText(text, 0, 0, layoutWidth, layoutHeight,
                       juce::Justification::centredLeft, true);
  }

private:
  juce::String currentTooltipText;
  juce::Image tooltipImage;
  juce::Point<int> lastMousePosition;
  Fonts fonts;
};

} // namespace window
} // namespace gui
} // namespace dmt