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

  void paint(juce::Graphics& g) override
  {
    // For debug: show green overlay if hovering a TooltipClient
    if (!tooltipImage.isNull())
      g.fillAll(juce::Colours::green.withAlpha(0.5f));
    else
      g.fillAll(juce::Colours::red.withAlpha(0.2f));
    // Do NOT draw the image to the screen yet
  }

  void repaintTimerCallback() noexcept override
  {
    auto* parent = getParentComponent();
    if (!parent)
      return;

    auto mousePos = parent->getMouseXYRelative();
    auto* comp = parent->getComponentAt(mousePos);

    juce::String foundTooltip;

    while (comp != nullptr && comp != parent)
    {
      if (auto* tooltipClient = dynamic_cast<juce::TooltipClient*>(comp))
      {
        auto tip = tooltipClient->getTooltip();
        if (!tip.isEmpty())
        {
          foundTooltip = tip;
          break;
        }
      }
      comp = comp->getParentComponent();
    }

    if (foundTooltip != currentTooltipText)
    {
      currentTooltipText = foundTooltip;
      if (currentTooltipText.isNotEmpty())
        renderTooltipImage(currentTooltipText);
      else
        tooltipImage = juce::Image(); // Null image
      repaint();
    }
  }

protected:
 void renderTooltipImage(const juce::String& text)
  {
    // For now, just render a simple white rectangle with the text in black
    constexpr int width = 300, height = 40;
    tooltipImage = juce::Image(juce::Image::ARGB, width, height, true);
    juce::Graphics g(tooltipImage);
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
    g.setFont(rawFontSize);
    g.drawText(text, tooltipImage.getBounds(), juce::Justification::centredLeft, true);
  }

private:
  juce::String currentTooltipText;
  juce::Image tooltipImage;

 
};

} // namespace window
} // namespace gui
} // namespace dmt