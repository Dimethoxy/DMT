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
  }
  inline const juce::String getName() noexcept
  {
    return "Channel " + juce::String(channel);
  }

private:
  const juce::String channel;
};
//==============================================================================
} // namespace panels
} // namespace gui
} // namespace dmt
