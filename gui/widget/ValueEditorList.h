//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/ValueEditor.h"
#include "dmt/utility/Settings.h"
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
  using SettingsEditorSettings = dmt::Settings::SettingsEditor;
  using String = juce::String;
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using ValueEditor = dmt::gui::component::ValueEditor;
  using ValueEditorPointer = std::unique_ptr<ValueEditor>;
  using ValueEditorPointerList = std::vector<ValueEditorPointer>;

  const float& size = Settings::Window::size;

  // TODO: Move to settings
  const float& rawFontSize = SettingsEditorSettings::fontSize;
  const Colour& seperatorColour = SettingsEditorSettings::seperatorColour;

public:
  ValueEditorList() { addAllEditors(); }

  ~ValueEditorList() override = default;

  void paint(juce::Graphics& _g) override
  {
    _g.setColour(seperatorColour);

    // Draw top line
    _g.drawLine(0.0f, 0.0f, static_cast<float>(getWidth()), 0.0f, 1.0f);

    // Draw separator lines at the top of each child component (except the first)
    for (int i = 1; i < getNumChildComponents(); ++i) {
      auto* comp = getChildComponent(i);
      if (comp) {
        float y = static_cast<float>(comp->getY());
        _g.drawLine(0.0f, y, static_cast<float>(getWidth()), y, 1.0f);
      }
    }

    // Draw bottom line at the bottom of the last child component
    if (getNumChildComponents() > 0) {
      auto* last = getChildComponent(getNumChildComponents() - 1);
      if (last) {
        float bottomY = static_cast<float>(last->getBottom());
        _g.drawLine(0.0f, bottomY, static_cast<float>(getWidth()), bottomY, 1.0f);
      }
    }
  }

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
    const auto extraHeight = fontSize * 0.5f;
    setSize(width, neededHeight + extraHeight);
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

    resized(); // Ensure layout is updated
    repaint(); // Ensure component is redrawn
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