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
    if (isHoveringTooltipClient)
      g.fillAll(juce::Colours::green.withAlpha(0.5f));
    else
      g.fillAll(juce::Colours::red.withAlpha(0.2f));
  }

  void repaintTimerCallback() noexcept override
  {
    auto* parent = getParentComponent();
    if (!parent)
      return;

    auto mousePos = parent->getMouseXYRelative();
    auto* comp = parent->getComponentAt(mousePos);

    isHoveringTooltipClient = false;
    juce::String foundTooltip;

    while (comp != nullptr && comp != parent)
    {
      if (auto* tooltipClient = dynamic_cast<juce::TooltipClient*>(comp))
      {
        auto tip = tooltipClient->getTooltip();
        if (!tip.isEmpty())
        {
          isHoveringTooltipClient = true;
          foundTooltip = tip;
          break;
        }
      }
      comp = comp->getParentComponent();
    }

    if (isHoveringTooltipClient)
      currentTooltipText = foundTooltip;
    else
      currentTooltipText = {};

    repaint();
  }

private:
  bool isHoveringTooltipClient = false;
  juce::String currentTooltipText;
};

} // namespace window
} // namespace gui
} // namespace dmt