//==============================================================================

#pragma once

//==============================================================================

#include "dmt/configuration/TreeAdapter.h"
#include "dmt/gui/component/AbstractDisplayComponent.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class SettingsEditorComponent
  : public dmt::gui::component::AbstractDisplayComponent
{
  using Image = juce::Image;
  using Settings = dmt::Settings;
  using TreeAdapter = dmt::configuration::TreeAdapter;

public:
  SettingsEditorComponent()
    : treeAdapter(Settings::container)
    , nextFrame(Image::ARGB, 1, 1, true)
  {
    // Minimal constructor
  }

  ~SettingsEditorComponent() override = default;

  void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    // Implement resizing logic here
  }

  void paintDisplay(juce::Graphics& _g,
                    const juce::Rectangle<int>& _displayBounds) const noexcept
  {
    // Implement painting logic here
    //_g.fillAll(juce::Colours::green);
  }

  void prepareNextFrame() noexcept override
  {
    // Implement frame preparation logic here
  }

private:
  TreeAdapter treeAdapter;
  Image nextFrame;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditorComponent)
};

} // namespace component
} // namespace gui
} // namespace dmt