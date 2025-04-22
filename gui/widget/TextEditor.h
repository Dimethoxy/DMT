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
  }

  ~TextEditor() override = default;

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextEditor)
};

} // namespace widget
} // namespace gui
} // namespace dmt