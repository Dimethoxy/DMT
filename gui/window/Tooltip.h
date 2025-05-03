#pragma once

//==============================================================================

#include "dmt/gui/widget/Shadow.h"
#include "dmt/utility/Fonts.h"
#include "dmt/utility/RepaintTimer.h"
#include "dmt/utility/Scaleable.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

class Tooltip
  : public juce::Component
  , public dmt::utility::RepaintTimer
  , public dmt::Scaleable
{
  template<typename T>
  using Rectangle = juce::Rectangle<T>;
  using Settings = dmt::Settings;
  using Fonts = dmt::utility::Fonts;
  using TooltipSettings = dmt::Settings::Tooltip;
  using Shadow = dmt::gui::widget::Shadow;

  //==============================================================================
  // Window
  const float& size = Settings::Window::size;

  // General
  const float& unixScale = Settings::unixFontScalingFactor;

  // Tooltip
  const Colour& backgroundColour = TooltipSettings::backgroundColour;
  const Colour& borderColour = TooltipSettings::borderColour;
  const Colour& fontColour = TooltipSettings::fontColour;
  const Colour& innerShadowColour = TooltipSettings::innerShadowColour;
  const Colour& outerShadowColour = TooltipSettings::outerShadowColour;
  const float& rawCornerRadius = TooltipSettings::cornerRadius;
  const float& rawBorderWidth = TooltipSettings::borderWidth;
  const float& innerShadowRadius = TooltipSettings::innerShadowRadius;
  const float& outerShadowRadius = TooltipSettings::outerShadowRadius;
  const float& rawFontSize = TooltipSettings::fontSize;
  const float& rawTextHorizontalPadding =
    TooltipSettings::textHorizontalPadding;
  const float& rawTextVerticalPadding = TooltipSettings::textVerticalPadding;
  const bool& drawOuterShadow = TooltipSettings::drawOuterShadow;
  const bool& drawInnerShadow = TooltipSettings::drawInnerShadow;

public:
  Tooltip() noexcept
    : outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    TRACER("Tooltip::Tooltip");
    setInterceptsMouseClicks(false, false);
    startRepaintTimer(); // Use custom repaint timer
  }

  ~Tooltip() override = default;

  void resized() override
  {
    TRACER("Tooltip::resized");
    if (!tooltipImage.isNull())
      renderTooltipImage(currentTooltipText);
  }

  void paint(juce::Graphics& graphics) override
  {
    TRACER("Tooltip::paint");
    if (!tooltipImage.isNull()) {
      const float scale = getScaleFactor(this);
      const int imageWidth = tooltipImage.getWidth();
      const int imageHeight = tooltipImage.getHeight();
      const int width = getWidth();
      const int height = getHeight();

      int drawPositionX = lastMousePosition.x;
      int drawPositionY = lastMousePosition.y;

      // If drawing at (x, y) would go off the right edge, flip to left of mouse
      if (drawPositionX + imageWidth / scale > width)
        drawPositionX =
          std::max(0, lastMousePosition.x - (int)(imageWidth / scale));

      // If drawing at (x, y) would go off the bottom, flip to above mouse
      if (drawPositionY + imageHeight / scale > height)
        drawPositionY =
          std::max(0, lastMousePosition.y - (int)(imageHeight / scale));

      graphics.drawImage(tooltipImage,
                         (float)drawPositionX,
                         (float)drawPositionY,
                         imageWidth / scale,
                         imageHeight / scale,
                         0,
                         0,
                         imageWidth,
                         imageHeight);
    }
  }

  void repaintTimerCallback() noexcept override
  {
    TRACER("Tooltip::repaintTimerCallback");
    // Get the parent component of the tooltip
    auto* parent = getParentComponent();
    if (!parent)
      return;

    // Get the current mouse position relative to the parent component
    auto mousePosition = parent->getMouseXYRelative();
    auto* component = parent->getComponentAt(mousePosition);

    juce::String foundTooltipText;

    // Traverse up the component hierarchy to find a TooltipClient
    while (component != nullptr && component != parent) {
      if (auto* tooltipClient = dynamic_cast<juce::TooltipClient*>(component)) {
        // If a TooltipClient is found, get its tooltip text
        auto tooltipText = tooltipClient->getTooltip();
        if (!tooltipText.isEmpty()) {
          foundTooltipText = tooltipText;
          break;
        }
      }
      component = component->getParentComponent();
    }

    bool needsRepaint = false;

    // Check if the tooltip text has changed
    if (foundTooltipText != currentTooltipText) {
      currentTooltipText = foundTooltipText;

      // If there is new tooltip text, render the tooltip image
      if (currentTooltipText.isNotEmpty())
        renderTooltipImage(currentTooltipText);
      else
        tooltipImage = juce::Image(); // Clear the tooltip image if no text

      needsRepaint = true;
    }

    // Check if the mouse position has changed
    if (mousePosition != lastMousePosition) {
      lastMousePosition = mousePosition;
      needsRepaint = true;
    }

    // Repaint the tooltip if necessary
    if (needsRepaint)
      repaint();
  }

protected:
  void renderTooltipImage(const juce::String& text)
  {
    TRACER("Tooltip::renderTooltipImage");
    // Font size and style
    const auto fontSize = rawFontSize * size;

    // Determine font size based on OS
    const auto font =
      fonts.medium.withHeight(OS_IS_WINDOWS ? fontSize : fontSize * unixScale);

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
    const auto borderWidth = rawBorderWidth * size;
    const auto outerWidth = innerWidth + borderWidth * 2;
    const auto outerHeight = innerHeight + borderWidth * 2;
    const auto shadowRadius =
      std::max(outerShadowRadius, innerShadowRadius) * size;
    const auto tooltipWidth = outerWidth + shadowRadius * 2;
    const auto tooltipHeight = outerHeight + shadowRadius * 2;
    const auto cornerRadius = rawCornerRadius * size;
    const auto innerCornerRadius = cornerRadius - borderWidth;

    // HiDPI scaling stuff
    const float scale = getScaleFactor(this);
    const auto scaledTooltipWidth = juce::roundToInt(tooltipWidth * scale);
    const auto scaledTooltipHeight = juce::roundToInt(tooltipHeight * scale);

    tooltipImage = juce::Image(
      juce::Image::ARGB, scaledTooltipWidth, scaledTooltipHeight, true);
    juce::Graphics graphics(tooltipImage);
    graphics.addTransform(juce::AffineTransform::scale(scale, scale));

    // Bounds
    const auto tooltipBounds =
      Rectangle<float>(0, 0, tooltipWidth, tooltipHeight);
    const auto outerBounds = tooltipBounds.reduced(shadowRadius);
    const auto innerBounds = outerBounds.reduced(borderWidth);
    const auto textBounds =
      innerBounds.reduced(textHorizontalPadding, textVerticalPadding);

    // Path for the tooltip shape (outer and inner)
    juce::Path outerPath, innerPath;
    outerPath.addRoundedRectangle(outerBounds, cornerRadius);
    innerPath.addRoundedRectangle(innerBounds, innerCornerRadius);

    // Draw outer shadow if enabled
    if (drawOuterShadow)
      outerShadow.directDraw(graphics, outerPath);

    // Draw border
    graphics.setColour(borderColour);
    graphics.fillPath(outerPath);

    // Draw inner shadow if enabled
    if (drawInnerShadow)
      innerShadow.directDraw(graphics, innerPath);

    // Background
    graphics.setColour(backgroundColour);
    graphics.fillPath(innerPath);

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

  // Shadow members
  Shadow outerShadow;
  Shadow innerShadow;
};

} // namespace window
} // namespace gui
} // namespace dmt