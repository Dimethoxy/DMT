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
  using SettingsEditorSettings = dmt::Settings::SettingsEditor;

  //==============================================================================
  // Window
  const float& size = Settings::Window::size;

  // SettingsEditor
  const float& labelHorizontalPadding = SettingsEditorSettings::labelHorizontalPadding;
  const float& fontSize = SettingsEditorSettings::fontSize;
  const Colour& fontColour = SettingsEditorSettings::fontColour;

public:
  class Listener
  {
    public:
    virtual void valueEditorListenerCallback() = 0;  
  };
public:
  ValueEditor(TreeAdapter::Leaf& _leaf)
    : leaf(_leaf)
    , label(String(leaf.name),
            fonts.medium,
            fontSize,
            fontColour,
            juce::Justification::centredLeft)
    , editor(String(leaf.name + "Editor"))
  {
    TRACER("ValueEditor::ValueEditor");
    addAndMakeVisible(label);
    addAndMakeVisible(editor);


    editor.setText(String(leaf.toString()));
    setStyle();

    editor.onFocusLost = [this]() { newValueCallback(); };
    editor.onReturnKey = [this]() { newValueCallback(); };
  }

  ~ValueEditor() override = default;

  void paint(juce::Graphics& _g) override
  {
    TRACER("ValueEditor::paint");
    if (!editor.hasKeyboardFocus(false)) {
      editor.setText(leaf.toString(), juce::dontSendNotification);
    }
  }

  void resized() override
  {
    TRACER("ValueEditor::resized");
    auto bounds = getLocalBounds();
    auto labelBounds = bounds.removeFromLeft(bounds.getWidth() / 3.0f * 2.0f);
    auto editorBounds = bounds;
    label.setBounds(labelBounds);
    editor.setBounds(editorBounds);
    editor.setFont(fonts.medium.withHeight(fontSize * size));
    editor.setText("", juce::dontSendNotification);
    editor.setText(leaf.toString(), juce::dontSendNotification);
  }

  // TODO: This is absolutely horrible
  void setStyle(){
    TRACER("ValueEditor::setStyle");
    label.setRawHorizontalPadding(labelHorizontalPadding);
    editor.setColour(juce::TextEditor::textColourId, fontColour);
    editor.setColour(juce::TextEditor::backgroundColourId,
                  juce::Colours::transparentBlack);
    editor.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
  }

  void newValueCallback()
  {
    TRACER("ValueEditor::newValueCallback");
    auto newText = editor.getText();
    if (!leaf.parseAndSet(newText)) {
      editor.setText(leaf.toString(), juce::dontSendNotification);
      return;
    }
    // Let's trigger a resize of the top level component
    // const auto topLevelBounds = getTopLevelComponent()->getBounds();
    // const auto width = topLevelBounds.getWidth();
    // const auto height = topLevelBounds.getHeight();
    // getTopLevelComponent()->setSize(0,0);
    // getTopLevelComponent()->setSize(width, height);
    // Iterate parents to find the compositor
    auto* parent = getParentComponent();
    while (dynamic_cast<Listener*>(parent) == nullptr) {
      parent = parent->getParentComponent();
      if (parent == nullptr) {
        break;
      }
    }
    if (parent != nullptr) {
      Listener* listener = dynamic_cast<Listener*>(parent);
      listener->valueEditorListenerCallback();
    }

    auto* topLevelComponent = getTopLevelComponent();
    topLevelComponent->repaint();
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