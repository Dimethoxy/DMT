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

  const float& size = Settings::Window::size;

  const float fontSize = 16.0f;
  const Colour fontColour = juce::Colours::white;

public:
  ValueEditor(const String _name)
    : label(String(_name + "Label"),
            fonts.medium,
            fontSize,
            fontColour,
            juce::Justification::topLeft)
    , editor(String(_name + "Editor"))
  {
    addAndMakeVisible(label);
    addAndMakeVisible(editor);
  }

  ~ValueEditor() override = default;

  void paint(juce::Graphics& _g) override { _g.fillAll(Colours::darkgreen); }

  void resized() override
  {
    auto bounds = getLocalBounds();
    auto labelBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
    auto editorBounds = bounds;
  }

private:
  //==============================================================================
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