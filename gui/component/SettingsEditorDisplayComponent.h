//==============================================================================

#pragma once

//==============================================================================

#include "dmt/configuration/TreeAdapter.h"
#include "dmt/gui/component/AbstractDisplayComponent.h"
#include "dmt/gui/widget/TextEditor.h"
#include "dmt/gui/widget/ValueEditorList.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class SettingsEditorDisplayComponent
  : public dmt::gui::component::AbstractDisplayComponent
{
  using Image = juce::Image;
  using Settings = dmt::Settings;
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using TextEditor = dmt::gui::widget::TextEditor;
  using ValueEditorList = dmt::gui::component::ValueEditorList;

  const float& size = Settings::Window::size;

public:
  SettingsEditorDisplayComponent()
    : treeAdapter(Settings::container)
    , searchEditor("TestEditor")
  {
    addAndMakeVisible(searchEditor);
    addAndMakeVisible(valueEditorList);
  }

  ~SettingsEditorDisplayComponent() override = default;

  void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    auto bounds = _displayBounds.reduced(5.0f * size);
    const auto testBounds = bounds.removeFromTop(24 * size);
    searchEditor.setBounds(testBounds);
    const auto editorBounds = bounds;
    valueEditorList.setBounds(editorBounds);
  }

  void paintDisplay(juce::Graphics& _g,
                    const juce::Rectangle<int>& _displayBounds) const noexcept
  {
  }

  void prepareNextFrame() noexcept override
  {
    // Implement frame preparation logic here
  }

private:
  TreeAdapter treeAdapter;
  TextEditor searchEditor;
  ValueEditorList valueEditorList;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditorDisplayComponent)
};

} // namespace component
} // namespace gui
} // namespace dmt