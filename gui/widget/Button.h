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
class HeaderCallbackButton : public dmt::gui::widget::AbstractHeaderButton
{
  using Settings = dmt::Settings;
  using Colour = juce::Colour;
  using Justification = juce::Justification;
  using ButtonSettings = dmt::Settings::Button;
  using String = juce::String;
  using AbstractButton = dmt::gui::widget::AbstractHeaderButton;

public:
  HeaderCallbackButton(juce::String _name, juce::String _iconName)
    : AbstractHeaderButton(_name, _iconName)
  {
    setPassiveState(); // Set initial state
  }

  ~HeaderCallbackButton() override = default;

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