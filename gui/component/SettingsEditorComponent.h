//==============================================================================

#pragma once

//==============================================================================

#include "dmt/configuration/TreeAdapter.h"
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
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using ValueEditorList = dmt::gui::component::ValueEditorList;
  using ValueCategoryList = dmt::gui::component::ValueCategoryList;
  using Component = juce::Component;

  const float& size = Settings::Window::size;

  const float rawScrollBarThickness = 8.0f;
  const float rawFontSize = 16.0f;

public:
  SettingsEditor()
    : treeAdapter(Settings::container)
    , searchEditor("TestEditor")
    , valueCategoryList(treeAdapter.getCategories(),
                        [this](const TreeAdapter::Category& category) {
                          onCategorySelectedCallback(category);
                        })

  {
    addAndMakeVisible(searchEditor);
    addAndMakeVisible(categoryViewport);
    addAndMakeVisible(editorViewport);

    categoryViewport.setViewedComponent(&valueCategoryList, false);
    categoryViewport.setScrollBarsShown(true, false, false, false);
    editorViewport.setViewedComponent(&valueEditorList, false);
    editorViewport.setScrollBarsShown(true, false, false, false);
    setScrollbarThicknesses();
    setScrollBarColour();
  }

  ~SettingsEditor() override = default;

  void paint(juce::Graphics& _g) override {}

  void resized() override
  {
    auto bounds = getLocalBounds();

    // Set bounds for the search editor
    const auto fontSize = static_cast<int>(rawFontSize * size);
    searchEditor.setBounds(bounds.removeFromTop(fontSize));

    // Calculate and set bounds for the category viewport
    const int categoryWidth = static_cast<int>(bounds.getWidth() * 0.35f);
    categoryViewport.setBounds(bounds.removeFromLeft(categoryWidth));
    layoutViewport(categoryViewport, valueCategoryList);

    // Set bounds for the editor viewport
    editorViewport.setBounds(bounds);
    layoutViewport(editorViewport, valueEditorList);

    // Update scrollbar thicknesses
    setScrollbarThicknesses();
  }

  void onCategorySelectedCallback(const TreeAdapter::Category& category)
  {
    std::cout << "Selected category: " << category.name << std::endl;
    valueEditorList.setCategory(category);
    valueEditorList.setOptimalSize(editorViewport.getWidth());
  }

protected:
  template<typename ComponentType>
  void layoutViewport(Viewport& viewport, ComponentType& content)
  {
    const int optimalWidth =
      viewport.isVerticalScrollBarShown()
        ? viewport.getWidth() - viewport.getScrollBarThickness()
        : viewport.getWidth();
    content.setOptimalSize(optimalWidth);
  }

  void setScrollbarThicknesses()
  {
    const int scrollBarThickness =
      static_cast<int>(rawScrollBarThickness * size);
    categoryViewport.setScrollBarThickness(scrollBarThickness);
    editorViewport.setScrollBarThickness(scrollBarThickness);
  }

  void setScrollBarColour()
  {
    categoryViewport.getVerticalScrollBar().setColour(
      juce::ScrollBar::ColourIds::thumbColourId, juce::Colours::white);
    editorViewport.getVerticalScrollBar().setColour(
      juce::ScrollBar::ColourIds::thumbColourId, juce::Colours::white);
  }

private:
  TreeAdapter treeAdapter;
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