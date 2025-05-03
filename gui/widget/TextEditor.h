//==============================================================================
#pragma once
//==============================================================================

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace widget {

//==============================================================================

class TextEditor : public juce::TextEditor
{
  using String = juce::String;

public:
  TextEditor(const String _name)
    : juce::TextEditor(_name)
  {
    setMultiLine(false);
    setFont(juce::Font(16.0f));
    setJustification(juce::Justification::centredLeft);
    setEscapeAndReturnKeysConsumed(true);
    setWantsKeyboardFocus(true);
  }

  ~TextEditor() override = default;

  bool keyPressed(const juce::KeyPress& key) override
  {
    if (key == juce::KeyPress::backspaceKey) {
      if (getText().isNotEmpty()) {
        setText(getText().dropLastCharacters(1));
      }
      return true;
    } else if (key == juce::KeyPress::returnKey) {
      // Handle return key press
    } else {
      // Handle other key presses
      return false;
    }
  }

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextEditor)
};

} // namespace widget
} // namespace gui
} // namespace dmt