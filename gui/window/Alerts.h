#pragma once

//==============================================================================

#include "dmt/gui/widget/Shadow.h"
#include "dmt/utility/Fonts.h"
#include "dmt/utility/RepaintTimer.h"
#include "dmt/utility/Scaleable.h"
#include "dmt/utility/Settings.h"
#include "utility/Icon.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

//==============================================================================

class Alerts
  : public juce::Component
  , public dmt::utility::RepaintTimer
  , public dmt::Scaleable
{
  template<typename T>
  using Rectangle = juce::Rectangle<T>;
  using Settings = dmt::Settings;
  using Fonts = dmt::utility::Fonts;
  using Shadow = dmt::gui::widget::Shadow;
  using AlertSettings = dmt::Settings::Alerts;

  //==============================================================================
  // Window
  const float& size = Settings::Window::size;

  // Alerts
  const Colour& warningBackgroundColour =
    AlertSettings::warningBackgroundColour;
  const Colour& warningBorderColour = AlertSettings::warningBorderColour;
  const Colour& warningFontColour = AlertSettings::warningFontColour;
  const Colour& warningIconColour = AlertSettings::warningIconColour;
  const Colour& warningOuterShadowColour =
    AlertSettings::warningOuterShadowColour;
  const Colour& warningInnerShadowColour =
    AlertSettings::warningInnerShadowColour;

  const Colour& errorBackgroundColour = AlertSettings::errorBackgroundColour;
  const Colour& errorBorderColour = AlertSettings::errorBorderColour;
  const Colour& errorFontColour = AlertSettings::errorFontColour;
  const Colour& errorIconColour = AlertSettings::errorIconColour;
  const Colour& errorOuterShadowColour = AlertSettings::errorOuterShadowColour;
  const Colour& errorInnerShadowColour = AlertSettings::errorInnerShadowColour;

  const Colour& infoBackgroundColour = AlertSettings::infoBackgroundColour;
  const Colour& infoBorderColour = AlertSettings::infoBorderColour;
  const Colour& infoFontColour = AlertSettings::infoFontColour;
  const Colour& infoIconColour = AlertSettings::infoIconColour;
  const Colour& infoOuterShadowColour = AlertSettings::infoOuterShadowColour;
  const Colour& infoInnerShadowColour = AlertSettings::infoInnerShadowColour;

  const Colour& successBackgroundColour =
    AlertSettings::successBackgroundColour;
  const Colour& successBorderColour = AlertSettings::successBorderColour;
  const Colour& successFontColour = AlertSettings::successFontColour;
  const Colour& successIconColour = AlertSettings::successIconColour;
  const Colour& successOuterShadowColour =
    AlertSettings::successOuterShadowColour;
  const Colour& successInnerShadowColour =
    AlertSettings::successInnerShadowColour;

  const float& rawCornerRadius = AlertSettings::cornerRadius;
  const float& rawBorderWidth = AlertSettings::borderWidth;
  const float& innerShadowRadius = AlertSettings::innerShadowRadius;
  const float& outerShadowRadius = AlertSettings::outerShadowRadius;
  const float& rawFontSize = AlertSettings::titleFontSize;
  const float& rawMessageFontSize = AlertSettings::messageFontSize;
  const float& rawTextHorizontalPadding = AlertSettings::textHorizontalPadding;
  const float& rawTextVerticalPadding = AlertSettings::textVerticalPadding;
  const float& rawIconSize = AlertSettings::iconSize;
  const float& rawContentSpacing = AlertSettings::contentSpacing;
  const int& rawAlertWidth = AlertSettings::alertWidth;
  const int& rawAlertHeight = AlertSettings::alertHeight;
  const float& maxAge = AlertSettings::maxAge;           // in seconds
  const float& fadeOutTime = AlertSettings::fadeOutTime; // in seconds
  const bool& drawOuterShadow = AlertSettings::drawOuterShadow;
  const bool& drawInnerShadow = AlertSettings::drawInnerShadow;

public:
  enum class AlertType
  {
    Info,
    Warning,
    Error,
    Success
  };

protected:
  struct AlertData
  {
    juce::String title;
    juce::String message;
    juce::String iconName;
    AlertType type;
    float age; // in seconds, used for fading out and removing alerts
    juce::Image cachedComponentImage;
  };

public:
  Alerts() noexcept
  {
    TRACER("Alerts::Alerts");
    setInterceptsMouseClicks(false, false);
    startRepaintTimer();
  }

  // Push a new alert to the list
  void pushAlert(const juce::String title,
                 const juce::String message,
                 AlertType type,
                 const juce::String iconName = {})
  {
    TRACER("Alerts::pushAlert");
    // When we push a new alert, let old ones age quicker
    const auto quickAgeTarget = maxAge - fadeOutTime;
    for (int i = 0; i < alerts.size(); ++i)
      if (alerts.getReference(i).age < quickAgeTarget)
        alerts.getReference(i).age = quickAgeTarget;

    AlertData alert{ title, message, iconName, type, 0.0f };
    renderAlertToImage(alert);
    alerts.add(alert);
    repaint();
  }

  void resized() override
  {
    TRACER("Alerts::resized");
    // Re-render all alert images on resize
    for (int i = 0; i < alerts.size(); ++i)
      renderAlertToImage(alerts.getReference(i));
    repaint();
  }

  void paint(juce::Graphics& g) override
  {
    TRACER("Alerts::paint");
    if (alerts.size() == 0)
      return;

    // Calculate scaled alert size and spacing
    const auto alertWidth = rawAlertWidth * size;
    const auto alertHeight = rawAlertHeight * size;
    const int spacing = int(8 * size);
    const int marginBottom = int(24 * size);

    const int totalHeight =
      alerts.size() * alertHeight +
      (alerts.size() > 0 ? (alerts.size() - 1) * spacing : 0);
    const int areaWidth = getWidth();
    const int areaHeight = getHeight();
    int y = areaHeight - totalHeight - marginBottom;

    for (const auto& alert : alerts) {
      float alpha = 1.0f;
      if (alert.age > maxAge - fadeOutTime)
        alpha = juce::jlimit(0.0f, 1.0f, (maxAge - alert.age) / fadeOutTime);

      g.setOpacity(alpha);
      int x = (areaWidth - alertWidth) / 2;
      g.drawImage(alert.cachedComponentImage,
                  (float)x,
                  (float)y,
                  (float)alertWidth,
                  (float)alertHeight,
                  0,
                  0,
                  alert.cachedComponentImage.getWidth(),
                  alert.cachedComponentImage.getHeight());
      y += alertHeight + spacing;
    }
    g.setOpacity(1.0f);
  }

  void repaintTimerCallback() noexcept override
  {
    TRACER("Alerts::repaintTimerCallback");
    for (int i = alerts.size(); --i >= 0;) {
      alerts.getReference(i).age += Settings::framerate / 1000.0f;
      if (alerts.getReference(i).age >= maxAge)
        alerts.remove(i);
    }
    if (alerts.size() > 0)
      repaint();
  }

protected:
  // Helper to render an alert into its cached image
  void renderAlertToImage(AlertData& alert)
  {
    TRACER("Alerts::renderAlertToImage");
    const auto alertWidth = rawAlertWidth * size;
    const auto alertHeight = rawAlertHeight * size;

    // HiDPI support: render at higher resolution
    const float scale = getScaleFactor(this);
    const int hiResWidth = static_cast<int>(alertWidth * scale);
    const int hiResHeight = static_cast<int>(alertHeight * scale);

    alert.cachedComponentImage =
      juce::Image(juce::Image::ARGB, hiResWidth, hiResHeight, true);
    juce::Graphics g(alert.cachedComponentImage);
    g.addTransform(juce::AffineTransform::scale(scale, scale));

    // copy the colours to use
    Colour backgroundColour, borderColour, fontColour, iconColour,
      outerShadowColour, innerShadowColour;
    switch (alert.type) {
      case AlertType::Info:
        backgroundColour = infoBackgroundColour;
        borderColour = infoBorderColour;
        fontColour = infoFontColour;
        iconColour = infoIconColour;
        outerShadowColour = infoOuterShadowColour;
        innerShadowColour = infoInnerShadowColour;
        break;
      case AlertType::Warning:
        backgroundColour = warningBackgroundColour;
        borderColour = warningBorderColour;
        fontColour = warningFontColour;
        iconColour = warningIconColour;
        outerShadowColour = warningOuterShadowColour;
        innerShadowColour = warningInnerShadowColour;
        break;
      case AlertType::Error:
        backgroundColour = errorBackgroundColour;
        borderColour = errorBorderColour;
        fontColour = errorFontColour;
        iconColour = errorIconColour;
        outerShadowColour = errorOuterShadowColour;
        innerShadowColour = errorInnerShadowColour;
        break;
      case AlertType::Success:
        backgroundColour = successBackgroundColour;
        borderColour = successBorderColour;
        fontColour = successFontColour;
        iconColour = successIconColour;
        outerShadowColour = successOuterShadowColour;
        innerShadowColour = successInnerShadowColour;
        break;
    }

    const auto iconSize = rawIconSize * size;
    const auto titleFontSize = rawFontSize * size;
    const auto messageFontSize = rawMessageFontSize * size;
    const auto titleFont = fonts.medium.withHeight(titleFontSize);
    const auto messageFont = fonts.regular.withHeight(messageFontSize);
    const auto borderWidth = rawBorderWidth * size;
    const auto cornerRadius = rawCornerRadius * size;
    const auto innerCornerRadius = cornerRadius - borderWidth;
    const auto textHorizontalPadding = rawTextHorizontalPadding * size;
    const auto textVerticalPadding = rawTextVerticalPadding * size;
    const auto alertBounds = Rectangle<float>(0, 0, alertWidth, alertHeight);
    const auto outerBounds = alertBounds.reduced(outerShadowRadius * size);
    const auto innerBounds = outerBounds.reduced(borderWidth);
    auto contentBounds =
      innerBounds.reduced(textHorizontalPadding, textVerticalPadding);

    // Draw Border
    g.setColour(borderColour);
    g.fillRoundedRectangle(outerBounds, cornerRadius);

    // Draw Background
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerRadius);

    // Draw Icon
    auto icon = icons::getIcon(alert.iconName);
    auto uniqueIconPadding = icons::getPadding(alert.iconName) * size;
    if (icon == nullptr) {
      switch (alert.type) {
        case AlertType::Info:
          icon = icons::getIcon("Info");
          uniqueIconPadding = icons::getPadding("Info") * size;
          break;
        case AlertType::Warning:
          icon = icons::getIcon("Warning");
          uniqueIconPadding = icons::getPadding("Warning") * size;
          break;
        case AlertType::Error:
          icon = icons::getIcon("Error");
          uniqueIconPadding = icons::getPadding("Error") * size;
          break;
        case AlertType::Success:
          icon = icons::getIcon("Success");
          uniqueIconPadding = icons::getPadding("Success") * size;
          break;
      }
    }
    const auto clonedIcon = icon->createCopy();
    const auto iconBoundsWidth = iconSize + 2 * uniqueIconPadding;
    const auto iconBounds = contentBounds.removeFromLeft(iconBoundsWidth);
    clonedIcon->replaceColour(juce::Colours::black, iconColour);
    clonedIcon->drawWithin(
      g, iconBounds, juce::RectanglePlacement::centred, 1.0f);

    // Title
    const auto contentSpacing = rawContentSpacing * size;
    const auto contentWidth = contentBounds.getWidth() - contentSpacing;
    const auto titleBoundsHeight = contentBounds.getHeight() / 2.0f;
    const auto titleBounds = contentBounds.removeFromTop(titleBoundsHeight)
                               .removeFromRight(contentWidth);
    g.setFont(titleFont);
    g.setColour(fontColour);
    g.drawFittedText(alert.title,
                     titleBounds.toNearestInt(),
                     juce::Justification::bottomLeft,
                     1);

    // Message
    const auto messageBounds = contentBounds.removeFromRight(contentWidth);
    g.setFont(messageFont);
    g.setColour(fontColour);
    g.drawFittedText(alert.message,
                     messageBounds.toNearestInt(),
                     juce::Justification::centredLeft,
                     1);
  }

private:
  juce::Array<AlertData> alerts; // Store active alerts
  Fonts fonts;
};
} // namespace window
} // namespace gui
} // namespace dmt