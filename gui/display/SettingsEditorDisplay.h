//==============================================================================

#pragma once

//==============================================================================

#include "dmt/configuration/TreeAdapter.h"
#include "dmt/gui/component/SettingsEditorComponent.h"
#include "dmt/gui/display/AbstractDisplay.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace display {
//==============================================================================
class SettingsEditorDisplay : public dmt::gui::display::AbstractDisplay
{
  using Image = juce::Image;
  using Settings = dmt::Settings;
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using TextEditor = dmt::gui::widget::TextEditor;
  using SettingsEditor = dmt::gui::component::SettingsEditor;

  const float& size = Settings::Window::size;

public:
  SettingsEditorDisplay()
    : treeAdapter(Settings::container)
  {
    addAndMakeVisible(settingsEditor);
  }

  ~SettingsEditorDisplay() override = default;

  void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    auto bounds = _displayBounds.reduced(5.0f * size);
    settingsEditor.setBounds(bounds);
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
  SettingsEditor settingsEditor;
  TreeAdapter treeAdapter;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditorDisplay)
};

} // namespace component
} // namespace gui
} // namespace dmt