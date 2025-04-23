//==============================================================================
#pragma once
//==============================================================================
#include "gui/component/LinearSliderComponent.h"
#include "gui/component/RotarySliderComponent.h"
#include "gui/display/SettingsEditorDisplay.h"
#include "gui/panel/AbstractPanel.h"
#include "utility/Settings.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
//==============================================================================
class SettingsPanel : public dmt::gui::panel::AbstractPanel
{
  using RotarySliderComponent = dmt::gui::component::RotarySliderComponent;
  using LinearSliderComponent = dmt::gui::component::LinearSliderComponent;
  using RotarySliderType = dmt::gui::widget::RotarySlider::Type;
  using LinearSliderType = dmt::gui::widget::LinearSlider::Type;
  using Unit = dmt::utility::Unit;
  using SettingsEditorDisplay = dmt::gui::display::SettingsEditorDisplay;
  using Settings = dmt::Settings;

  const float& size = Settings::Window::size;
  const float& rawPadding = Settings::Panel::padding;

public:
  SettingsPanel(/*juce::AudioProcessorValueTreeState& apvts*/)
    : AbstractPanel("Work in Progress")
  {
    setLayout({ 22, 60 });
    addAndMakeVisible(settingsEditor);
  }

  ~SettingsPanel() override = default;

  void extendResize() noexcept override
  {
    auto bounds = getLocalBounds();

    const float padding = rawPadding * size;
    auto editorBounds = bounds.reduced(padding);
    const float editorHorizontalPadding = 100.0f;
    const float editorTopPadding = 35.0f;
    const float editorBottomPadding = 15.0f;
    editorBounds.removeFromTop(editorTopPadding * size);
    editorBounds.removeFromBottom(editorBottomPadding * size);
    editorBounds.removeFromLeft(editorHorizontalPadding * size);
    editorBounds.removeFromRight(editorHorizontalPadding * size);
    settingsEditor.setBounds(editorBounds);
  }

private:
  SettingsEditorDisplay settingsEditor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};
//==============================================================================
} // namespace panel
} // namespace gui
} // namespace dmt