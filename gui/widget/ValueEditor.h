//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/Label.h"
#include "dmt/gui/widget/TextEditor.h"
#include "dmt/utility/Fonts.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class ValueEditor : public juce::Component
{
  using Colour = juce::Colour;
  using Fonts = dmt::utility::Fonts;
  using Label = dmt::gui::widget::Label;
  using Settings = dmt::Settings;
  using String = juce::String;
  using TextEditor = dmt::gui::widget::TextEditor;
  using TreeAdapter = dmt::configuration::TreeAdapter;

  const float& size = Settings::Window::size;

  const float fontSize = 16.0f;
  const Colour fontColour = juce::Colours::white;

public:
  ValueEditor(TreeAdapter::Leaf& _leaf)
    : leaf(_leaf)
    , label(String(leaf.name),
            fonts.medium,
            fontSize,
            fontColour,
            juce::Justification::topLeft)
    , editor(String(leaf.name + "Editor"))
  {
    addAndMakeVisible(label);
    addAndMakeVisible(editor);
    label.setRawHorizontalPadding(5.0f);

    editor.setText(String(leaf.toString()));

    editor.onFocusLost = [this]() { newValueCallback(); };
    editor.onReturnKey = [this]() { newValueCallback(); };
  }

  ~ValueEditor() override = default;

  void paint(juce::Graphics& _g) override {}

  void resized() override
  {
    auto bounds = getLocalBounds();
    auto labelBounds = bounds.removeFromLeft(bounds.getWidth() / 3.0f * 2.0f);
    auto editorBounds = bounds;
    label.setBounds(labelBounds);
    editor.setBounds(editorBounds);
  }

  void newValueCallback()
  {
    auto newText = editor.getText();
    if (!leaf.parseAndSet(newText)) {
      editor.setText(leaf.toString(), juce::dontSendNotification);
      getTopLevelComponent()->resized();
      getTopLevelComponent()->repaint();
    }
  }

private:
  //==============================================================================
  TreeAdapter::Leaf& leaf;
  Label label;
  TextEditor editor;

  //==============================================================================
  Fonts fonts;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueEditor)
};

} // namespace component
} // namespace gui
} // namespace dmt