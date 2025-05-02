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

  //==============================================================================
  // Alerts
  static inline float maxAge = 3.0f; // in seconds
  static inline float fadeOutTime = 0.5f; // in seconds
  static constexpr int alertWidth = 320;
  static constexpr int alertHeight = 80;

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
    setInterceptsMouseClicks(false, false);
    startRepaintTimer();
  }

  // Push a new alert to the list
  void pushAlert(const juce::String& title, const juce::String& message, AlertType type, const juce::String& iconName = {})
  {
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
    // Re-render all alert images on resize
    for (int i = 0; i < alerts.size(); ++i)
      renderAlertToImage(alerts.getReference(i));
    repaint();
  }

  void paint(juce::Graphics& g) override
  {
    // Calculate total height of all alerts
    const int totalHeight = alerts.size() * alertHeight + (alerts.size() > 0 ? (alerts.size() - 1) * 8 : 0);
    const int areaWidth = getWidth();
    const int areaHeight = getHeight();
    int y = areaHeight - totalHeight - 24; // 24px margin from bottom

    for (const auto& alert : alerts)
    {
      float alpha = 1.0f;
      if (alert.age > maxAge - fadeOutTime)
        alpha = juce::jlimit(0.0f, 1.0f, (maxAge - alert.age) / fadeOutTime);

      g.setOpacity(alpha);
      int x = (areaWidth - alertWidth) / 2;
      g.drawImageAt(alert.cachedComponentImage, x, y, false);
      y += alertHeight + 8;
    }
    g.setOpacity(1.0f);
  }

  void repaintTimerCallback() noexcept override
  {
    for (int i = alerts.size(); --i >= 0;)
    {
      alerts.getReference(i).age += Settings::framerate / 1000.0f;
      if (alerts.getReference(i).age >= maxAge)
        alerts.remove(i);
    }
    repaint();
  }

protected:
  // Helper to render an alert into its cached image
  void renderAlertToImage(AlertData& alert)
  {
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
    g.fillRoundedRectangle(g.getClipBounds().toFloat(), 12.0f);

    // Draw icon (placeholder: colored circle)
    g.setColour(juce::Colours::white);
    g.fillEllipse(16, 16, 32, 32);

    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(fonts.medium.withHeight(18.0f));
    g.drawText(alert.title, 56, 12, alertWidth - 64, 24, juce::Justification::left);

    // Draw message
    g.setFont(fonts.medium.withHeight(18.0f));
    g.setColour(juce::Colours::white.withAlpha(0.85f));
    g.drawText(alert.message, 56, 36, alertWidth - 64, 32, juce::Justification::left);

    // Optionally: draw shadow, border, etc.
  }
private:
 juce::Array<AlertData> alerts; // Store active alerts
 Fonts fonts;
};
} // namespace window
} // namespace gui
} // namespace dmt