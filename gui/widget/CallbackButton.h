#pragma once
//==============================================================================
#include "gui/widget/AbstractButton.h"
#include "gui/widget/Shadow.h"
#include "utility/Icon.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class CallbackButton : public dmt::gui::widget::AbstractButton
{
  using AbstractButton = dmt::gui::widget::AbstractButton;

public:
  CallbackButton(juce::String _name,
                 juce::String _iconName,
                 juce::String _tooltip = "",
                 bool shouldDrawBorder = true,
                 bool shouldDrawBackground = true,
                 bool shouldDrawShadow = true)
    : AbstractButton(_name, _iconName)
  {
    setPassiveState(); // Set initial state
  }

  ~CallbackButton() override = default;

  void mouseEnter(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      setHoverState();
    }
  }

  void mouseExit(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      setPassiveState();
    }
  }

  void mouseDown(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      setClickedState();
    }
  }

  void mouseUp(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      setHoverState();
      onClick(); // Trigger the button's click action
    }
  }
};
} // namespace widget
} // namespace dmt
} // namespace gui