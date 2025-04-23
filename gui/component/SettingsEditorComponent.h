//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/ValueEditorList.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class SettingsEditor : public juce::Component
{
  using Colour = juce::Colour;
  using Settings = dmt::Settings;
  using String = juce::String;
  using ValueEditorList = dmt::gui::component::ValueEditorList;

  const float& size = Settings::Window::size;

public:
  SettingsEditor()
    : searchEditor("TestEditor")
  {
    addAndMakeVisible(searchEditor);
    addAndMakeVisible(valueEditorList);
  }

  ~SettingsEditor() override = default;

  void paint(juce::Graphics& _g) override {}

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto testBounds = bounds.removeFromTop(24 * size);
    searchEditor.setBounds(testBounds);
    const auto editorBounds = bounds;
    valueEditorList.setOptimalSize(editorBounds.getWidth());
    valueEditorList.setTopLeftPosition(editorBounds.getPosition());
  }

private:
  TextEditor searchEditor;
  ValueEditorList valueEditorList;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditor)
};

} // namespace component
} // namespace gui
} // namespace dmt