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
  template <typename T>
  using Rectangle = juce::Rectangle<T>;
  using Settings = dmt::Settings;
  using Fonts = dmt::utility::Fonts;
  
  //==============================================================================
  // Window 
  const float& size = Settings::Window::size;

  // Tooltip
  const float rawFontSize = 18.0f;
  const float rawCornerRadius = 10.0f;
  const float rawBorderThickness = 2.0f;
  const Colour fontColour = juce::Colours::white;
  const Colour borderColour = juce::Colours::orange;
  const Colour backgroundColour = juce::Colours::darkgreen;
  const float rawTextHorizontalPadding = 10.0f;
  const float rawTextVerticalPadding = 5.0f;
  const float rawShadowRadius = 10.0f;
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
    // Font size and style
    const auto fontSize = rawFontSize * size;
    const auto font = fonts.medium.withHeight(fontSize);
    const auto justification = juce::Justification::centredLeft;

    // Prepare the AttributedString
    juce::AttributedString attributedString(text);
    attributedString.setFont(font);
    attributedString.setColour(fontColour);
    attributedString.setJustification(justification);
    attributedString.setWordWrap(juce::AttributedString::WordWrap::none);

    // Calculate the size of the text layout
    const float maxWidth = getWidth();
    juce::TextLayout textLayout;
    textLayout.createLayout(attributedString, maxWidth);
    const auto layoutWidth = textLayout.getWidth() * 1.01f;
    const auto layoutHeight = textLayout.getHeight() * 1.01f;
    
    // Precalculate parameters
    const auto textHorizontalPadding = rawTextHorizontalPadding * size;
    const auto textVerticalPadding = rawTextVerticalPadding * size;
    const auto innerWidth = layoutWidth + textHorizontalPadding * 2;
    const auto innerHeight = layoutHeight + textVerticalPadding * 2;    
    const auto borderThickness = rawBorderThickness * size;
    const auto outerWidth = innerWidth + borderThickness * 2;
    const auto outerHeight = innerHeight + borderThickness * 2;
    const auto shadowRadius = rawShadowRadius * size;
    const auto tooltipWidth = outerWidth + shadowRadius * 2;
    const auto tooltipHeight = outerHeight + shadowRadius * 2;
    const auto cornerRadius = rawCornerRadius * size;
    const auto innerCornerRadius = cornerRadius - borderThickness;

    // Bounds
    const auto tooltipBounds = Rectangle<float>(0, 0, tooltipWidth, tooltipHeight);
    const auto outerBounds = tooltipBounds.reduced(shadowRadius);
    const auto innerBounds = outerBounds.reduced(borderThickness);
    const auto textBounds = innerBounds.reduced(textHorizontalPadding, textVerticalPadding);
    
    tooltipImage = juce::Image(juce::Image::ARGB, tooltipWidth, tooltipHeight, true);
    juce::Graphics graphics(tooltipImage);
    
    // Border
    graphics.setColour(borderColour);
    graphics.fillRoundedRectangle(outerBounds, cornerRadius);
    
    // Background
    graphics.setColour(backgroundColour);
    graphics.fillRoundedRectangle(innerBounds, innerCornerRadius);
    
    // Text
    graphics.setColour(fontColour);
    graphics.setFont(font);
    graphics.drawText(text, textBounds, justification, true);
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