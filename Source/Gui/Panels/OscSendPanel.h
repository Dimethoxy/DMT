//==============================================================================

#pragma once

#include "Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class OscSendPanel : public dmt::gui::Panel
{
public:
  OscSendPanel(const juce::String channel)
    : channel(channel)
  {
    dmt::gui::Panel::Layout layout = { 1, 5 };
    setLayout(layout);
  }

  inline const juce::String getName() noexcept
  {
    return "Channel " + juce::String(channel);
  }

private:
  const juce::String channel;
  dmt::gui::components::LinearSliderComponent gainSlider;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
