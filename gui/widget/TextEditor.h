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
        // Remove the character in front of the caret
        const auto caretPos = getCaretPosition();
        if (caretPos > 0) {
          newTransaction();
          const auto text = getText();
          // split the text into two parts
          const auto firstPart = text.substring(0, caretPos - 1);
          const auto secondPart = text.substring(caretPos);
          // set the new text
          setText(firstPart + secondPart);
          // set the caret position
          setCaretPosition(caretPos - 1);
        }
      }
      return true;
    }

    // If the key is not handled, call the base class implementation
    return juce::TextEditor::keyPressed(key);
  }

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextEditor)
};

} // namespace widget
} // namespace gui
} // namespace dmt