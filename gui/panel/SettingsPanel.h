//==============================================================================
/* ██████╗ ██╗███╗   ███╗███████╗████████╗██╗  ██╗ ██████╗ ██╗  ██╗██╗   ██╗
 * ██╔══██╗██║████╗ ████║██╔════╝╚══██╔══╝██║  ██║██╔═══██╗╚██╗██╔╝╚██╗ ██╔╝
 * ██║  ██║██║██╔████╔██║█████╗     ██║   ███████║██║   ██║ ╚███╔╝  ╚████╔╝
 * ██║  ██║██║██║╚██╔╝██║██╔══╝     ██║   ██╔══██║██║   ██║ ██╔██╗   ╚██╔╝
 * ██████╔╝██║██║ ╚═╝ ██║███████╗   ██║   ██║  ██║╚██████╔╝██╔╝ ██╗   ██║
 * ╚═════╝ ╚═╝╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * Part of the Dimethoxy Library, primarily intended for Dimethoxy plugins.
 * External use is permitted but not recommended.
 * No support or compatibility guarantees are provided.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * SettingsPanel is a GUI component that provides controls for global settings.
 * It hosts a MultiDisplay (which provides chrome/border/shadow) containing a
 * single SettingsEditorDisplay. This avoids the diamond ambiguity that would
 * arise from having SettingsEditorDisplay extend both AbstractDisplay and
 * DisplayChrome directly.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include "gui/component/LinearSliderComponent.h"
#include "gui/component/RotarySliderComponent.h"
#include "gui/display/MultiDisplay.h"
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
  using Display = dmt::gui::display::AbstractDisplay;
  using DisplayPtr = std::unique_ptr<Display>;
  using DisplayList = std::vector<DisplayPtr>;
  using SettingsEditorDisplay = dmt::gui::display::SettingsEditorDisplay;
  using MultiDisplay = dmt::gui::display::MultiDisplay;
  using Settings = dmt::Settings;

  //==============================================================================
  const float& rawPadding = Settings::Panel::padding;

public:
  explicit SettingsPanel(juce::AudioProcessorValueTreeState& _apvts)
    : AbstractPanel("Settings", false)
    , settingsDisplay([&_apvts] {
        DisplayList displays;
        displays.push_back(std::make_unique<SettingsEditorDisplay>());
        return std::make_unique<MultiDisplay>(
          _apvts,
          std::move(displays),
          ParameterDisplayMap{});
      }())
  {
    TRACER("SettingsPanel::SettingsPanel");
    setLayout({ 22, 60 });
    addAndMakeVisible(settingsDisplay.get());
  }

  ~SettingsPanel() override = default;

  void extendResize() noexcept override
  {
    TRACER("SettingsPanel::extendResize");
    auto bounds = getLocalBounds();

    const float padding = rawPadding * size;
    auto editorBounds = bounds.reduced(padding);
    settingsDisplay->setBounds(editorBounds);
  }

private:
  using ParameterDisplayMap =
    std::unordered_map<juce::String, juce::String>;

  std::unique_ptr<MultiDisplay> settingsDisplay;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPanel)
};
//==============================================================================
} // namespace panel
} // namespace gui
} // namespace dmt