//==============================================================================

#pragma once

//==============================================================================

#include "dmt/configuration/TreeAdapter.h"
#include "dmt/gui/component/AbstractDisplayComponent.h"
#include "dmt/gui/widget/TextEditor.h"
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

  const float& size = Settings::Window::size;

public:
  SettingsEditorDisplayComponent()
    : treeAdapter(Settings::container)
    , searchEditor("TestEditor")
  {
    addAndMakeVisible(searchEditor);
  }

  ~SettingsEditorDisplayComponent() override = default;

  void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    auto bounds = _displayBounds;
    const auto testBounds = bounds.removeFromTop(24 * size);
    searchEditor.setBounds(testBounds);
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
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditorDisplayComponent)
};

} // namespace component
} // namespace gui
} // namespace dmt