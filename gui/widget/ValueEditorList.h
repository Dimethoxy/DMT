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
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using ValueEditor = dmt::gui::component::ValueEditor;
  using ValueEditorPointer = std::unique_ptr<ValueEditor>;
  using ValueEditorPointerList = std::vector<ValueEditorPointer>;

  const float& size = Settings::Window::size;

  // TODO: Move to settings
  const float rawFontSize = 16.0f;

public:
  ValueEditorList() { addAllEditors(); }

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

  void setCategory(TreeAdapter::Category& _category)
  {
    // Check if the category is the same as the current one
    if (category != nullptr && category->name == _category.name)
      return;

    // Remove all child components before clearing the list
    removeAllChildren();

    // Clear existing editors
    editorList.clear();

    // Generate new editors based on the selected category
    for (auto& leaf : _category.leaves) {
      auto editor = std::make_unique<ValueEditor>(leaf);
      editorList.push_back(std::move(editor));
    }

    // Add new editors to the component
    addAllEditors();

    // Set the new category
    category = &_category;
  }

protected:
  void addAllEditors()
  {
    for (auto& editor : editorList) {
      addAndMakeVisible(*editor);
    }
  }

  TreeAdapter::Category* category = nullptr;
  ValueEditorPointerList editorList;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditorList)
};

} // namespace component
} // namespace gui
} // namespace dmt