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
  using ValueEditorPointer = std::unique_ptr<ValueEditor>;
  using ValueEditorPointerList = std::vector<ValueEditorPointer>;

  const float& size = Settings::Window::size;

  // TODO: Move to settings
  const float rawFontSize = 24.0f;

public:
  ValueEditorList()
  {
    generateDummyEditors();
    addAllEditors();
  }

  ~ValueEditorList() override = default;

  void paint(juce::Graphics& _g) override { _g.fillAll(Colours::darkmagenta); }

  void resized() override
  {
    const auto fontSize = rawFontSize * size;

    auto bounds = getLocalBounds();
    for (auto& editor : editorList) {
      auto editorBounds = bounds.removeFromTop(fontSize);
      editor->setBounds(editorBounds);
    }
  }

  void setOptimalSize(const int width)
  {
    const auto fontSize = rawFontSize * size;
    const auto neededHeight = fontSize * editorList.size();
    setSize(width, neededHeight);
  }

protected:
  void generateDummyEditors()
  {
    for (int i = 0; i < 15; ++i) {
      const auto name = String("ValueEditor") + String(i);
      auto editor = std::make_unique<ValueEditor>(name);
      editorList.push_back(std::move(editor));
    }
  }
  void addAllEditors()
  {
    for (auto& editor : editorList) {
      addAndMakeVisible(*editor);
    }
  }

private:
  ValueEditorPointerList editorList;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditorList)
};

} // namespace component
} // namespace gui
} // namespace dmt