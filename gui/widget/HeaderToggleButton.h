#pragma once
//==============================================================================
#include "gui/widget/AbstractHeaderButton.h"
#include "gui/widget/Shadow.h"
#include "utility/Icon.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
class HeaderToggleButton
  : public dmt::gui::widget::AbstractHeaderButton
  , public juce::AudioProcessorValueTreeState::Listener
{
  using AbstractButton = dmt::gui::widget::AbstractHeaderButton;

public:
  HeaderToggleButton(juce::String _name,
                     juce::String _iconName,
                     juce::String _parameterID,
                     AudioProcessorValueTreeState& _apvts)
    : AbstractHeaderButton(_name, _iconName)
    , parameterID(_parameterID)
    , apvts(_apvts)
  {
    // Start listening to the parameter
    apvts.addParameterListener(parameterID, this);

    // Set the button's initial state based on the parameter value
    auto* bypassParameter = apvts.getParameter(parameterID);
    if (bypassParameter != nullptr) {
      auto bypassValue = bypassParameter->getValue();
      if (bypassValue == 0.0f) {
        setPassiveState();
      } else {
        setClickedState();
      }
    }
  }

  ~HeaderToggleButton() override
  {
    apvts.removeParameterListener(parameterID, this);
  }

  void mouseEnter(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      auto* bypassParameter = apvts.getParameter(parameterID);
      if (bypassParameter != nullptr && bypassParameter->getValue() == 0.0f) {
        setHoverState();
      }
    }
  }

  void mouseExit(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      auto* bypassParameter = apvts.getParameter(parameterID);
      if (bypassParameter != nullptr && bypassParameter->getValue() == 0.0f) {
        setPassiveState();
      }
    }
  }

  void mouseDown(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
    }
  }

  void mouseUp(const juce::MouseEvent& /*event*/) override
  {
    if (isEnabled()) {
      // Toggle the parameter value
      auto* bypassParameter = apvts.getParameter(parameterID);
      if (bypassParameter != nullptr) {
        auto newValue = bypassParameter->getValue() == 0.0f ? 1.0f : 0.0f;
        bypassParameter->setValueNotifyingHost(newValue);
      }
    }
  }

  void parameterChanged(const juce::String& _parameterID,
                        float _newValue) override
  {
    if (_parameterID == parameterID) {
      if (_newValue == 0.0f) {
        setPassiveState();
      } else {
        setClickedState();
      }
    }
  }

private:
  juce::String parameterID;
  AudioProcessorValueTreeState& apvts;
};
} // namespace widget
} // namespace dmt
} // namespace gui