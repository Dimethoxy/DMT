//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/ValueCategoryList.h"
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
  using Viewport = juce::Viewport;
  using ValueEditorList = dmt::gui::component::ValueEditorList;
  using ValueCategoryList = dmt::gui::component::ValueCategoryList;

  const float& size = Settings::Window::size;

public:
  SettingsEditor()
    : searchEditor("TestEditor")
  {
    addAndMakeVisible(searchEditor);
    addAndMakeVisible(categoryViewport);
    addAndMakeVisible(editorViewport);

    categoryViewport.setViewedComponent(&valueCategoryList, false);
    categoryViewport.setScrollBarsShown(true, false, false, false);
    categoryViewport.setScrollBarThickness(12 * size);

    editorViewport.setViewedComponent(&valueEditorList, false);
    editorViewport.setScrollBarsShown(true, false, false, false);
    editorViewport.setScrollBarThickness(12 * size);
  }

  ~SettingsEditor() override = default;

  void paint(juce::Graphics& _g) override {}

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto testBounds = bounds.removeFromTop(24 * size);
    searchEditor.setBounds(testBounds);
    const int editorWidth = bounds.getWidth() * 0.7f;
    const auto editorBounds = bounds.removeFromRight(editorWidth);
    editorViewport.setBounds(editorBounds);
    if (editorViewport.isVerticalScrollBarShown()) {
      valueEditorList.setOptimalSize(editorViewport.getWidth() -
                                     editorViewport.getScrollBarThickness());
    } else {
      valueEditorList.setOptimalSize(editorViewport.getWidth());
    }
    const auto categoryBounds = bounds;
    categoryViewport.setBounds(categoryBounds);
    if (categoryViewport.isVerticalScrollBarShown()) {
      valueCategoryList.setOptimalSize(
        categoryViewport.getWidth() - categoryViewport.getScrollBarThickness());
    } else {
      valueCategoryList.setOptimalSize(categoryViewport.getWidth());
    }
  }

private:
  TextEditor searchEditor;
  Viewport categoryViewport;
  Viewport editorViewport;
  ValueEditorList valueEditorList;
  ValueCategoryList valueCategoryList;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsEditor)
};
} // namespace component
} // namespace gui
} // namespace dmt