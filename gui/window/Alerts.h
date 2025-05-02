#pragma once

//==============================================================================

#include <JuceHeader.h>
#include "dmt/utility/RepaintTimer.h"
#include "dmt/utility/Settings.h"
#include "dmt/utility/Fonts.h"
#include "dmt/gui/widget/Shadow.h"

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

//==============================================================================

class Alerts : public juce::Component, public dmt::utility::RepaintTimer
{
  template <typename T>
  using Rectangle = juce::Rectangle<T>;
  using Settings = dmt::Settings;
  using Fonts = dmt::utility::Fonts;
  using Shadow = dmt::gui::widget::Shadow;
  using AlertSettings = dmt::Settings::Alerts;

  //==============================================================================
  // Window
  const float& size = Settings::Window::size;

  // Alerts
  const Colour& backgroundColour = AlertSettings::backgroundColour; 
  const Colour& borderColour = AlertSettings::borderColour;
  const Colour& fontColour = AlertSettings::fontColour;
  const Colour& innerShadowColour = AlertSettings::innerShadowColour;
  const Colour& outerShadowColour = AlertSettings::outerShadowColour;
  const float& rawCornerRadius = AlertSettings::cornerRadius;
  const float& rawBorderWidth = AlertSettings::borderWidth;
  const float& innerShadowRadius = AlertSettings::innerShadowRadius;
  const float& outerShadowRadius = AlertSettings::outerShadowRadius;
  const float& rawFontSize = AlertSettings::fontSize;
  const float& rawTextHorizontalPadding = AlertSettings::textHorizontalPadding;
  const float& rawTextVerticalPadding = AlertSettings::textVerticalPadding;
  const float& rawAlertWidth = AlertSettings::alertWidth;
  const float& rawAlertHeight = AlertSettings::alertHeight;
  const float& maxAge = AlertSettings::maxAge; // in seconds
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
  void pushAlert(const juce::String& title, const juce::String& message, AlertType type, const juce::String& iconName = {})
  {
    TRACER("Alerts::pushAlert");
    // When we push a new alert, let old ones age quicker
    const auto quickAgeTarget = maxAge - fadeOutTime;
    for (int i = 0; i < alerts.size(); ++i)
      if(alerts.getReference(i).age < quickAgeTarget)
        alerts.getReference(i).age = quickAgeTarget;

    AlertData alert { title, message, iconName, type, 0.0f };
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
    if(alerts.size() == 0)
      return;

    // Calculate scaled alert size and spacing
    const auto alertWidth = rawAlertWidth * size;
    const auto alertHeight = rawAlertHeight * size;
    const int spacing = int(8 * size);
    const int marginBottom = int(24 * size);

    const int totalHeight = alerts.size() * alertHeight + (alerts.size() > 0 ? (alerts.size() - 1) * spacing : 0);
    const int areaWidth = getWidth();
    const int areaHeight = getHeight();
    int y = areaHeight - totalHeight - marginBottom;

    for (const auto& alert : alerts)
    {
      float alpha = 1.0f;
      if (alert.age > maxAge - fadeOutTime)
        alpha = juce::jlimit(0.0f, 1.0f, (maxAge - alert.age) / fadeOutTime);

      g.setOpacity(alpha);
      int x = (areaWidth - alertWidth) / 2;
      g.drawImage(alert.cachedComponentImage, (float)x, (float)y, (float)alertWidth, (float)alertHeight, 
                  0, 0, alert.cachedComponentImage.getWidth(), alert.cachedComponentImage.getHeight());
      y += alertHeight + spacing;
    }
    g.setOpacity(1.0f);
  }

  void repaintTimerCallback() noexcept override
  {
    TRACER("Alerts::repaintTimerCallback");
    for (int i = alerts.size(); --i >= 0;)
    {
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
    alert.cachedComponentImage = juce::Image(juce::Image::ARGB, alertWidth, alertHeight, true);
    juce::Graphics g(alert.cachedComponentImage);

    // Background color based on alert type
    juce::Colour bg;
    switch (alert.type)
    {
      case AlertType::Info:    bg = juce::Colours::lightblue; break;
      case AlertType::Warning: bg = juce::Colours::orange;    break;
      case AlertType::Error:   bg = juce::Colours::red;       break;
      case AlertType::Success: bg = juce::Colours::green;     break;
      default:                 bg = juce::Colours::grey;      break;
    }

    g.setColour(bg.withAlpha(0.95f));
    g.fillRoundedRectangle(g.getClipBounds().toFloat(), 12.0f * size);

    // Draw icon (placeholder: colored circle)
    g.setColour(juce::Colours::white);
    g.fillEllipse(16 * size, 16 * size, 32 * size, 32 * size);

    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(fonts.medium.withHeight(18.0f * size));
    g.drawText(alert.title, int(56 * size), int(12 * size), int(alertWidth - 64 * size), int(24 * size), juce::Justification::left);

    // Draw message
    g.setFont(fonts.medium.withHeight(18.0f * size));
    g.setColour(juce::Colours::white.withAlpha(0.85f));
    g.drawText(alert.message, int(56 * size), int(36 * size), int(alertWidth - 64 * size), int(32 * size), juce::Justification::left);

    // Optionally: draw shadow, border, etc.
  }
private:
 juce::Array<AlertData> alerts; // Store active alerts
 Fonts fonts;
};
} // namespace window
} // namespace gui
} // namespace dmt