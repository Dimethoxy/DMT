//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/ValueEditor.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class ValueEditorList : public juce::Component
{
  using Colour = juce::Colour;
  using Settings = dmt::Settings;
  using String = juce::String;
  using ValueEditor = dmt::gui::component::ValueEditor;

  const float& size = Settings::Window::size;

public:
  ValueEditorList()
    : valueEditor("TestValue")
  {
    addAndMakeVisible(valueEditor);
  }

  ~ValueEditorList() override = default;

  void paint(juce::Graphics& _g) override { _g.fillAll(Colours::darkmagenta); }

  void resized() override {}

private:
  ValueEditor valueEditor;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditorList)
};

} // namespace component
} // namespace gui
} // namespace dmt