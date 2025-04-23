//==============================================================================

#pragma once

//==============================================================================

#include "dmt/gui/widget/ValueEditor.h"
#include "dmt/utility/Fonts.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class ValueCategoryList : public juce::Component
{
  using Colour = juce::Colour;
  using Settings = dmt::Settings;
  using String = juce::String;
  using Fonts = dmt::utility::Fonts;
  using Lable = dmt::gui::widget::Label;
  using LabelPointer = std::unique_ptr<Lable>;
  using LabelList = std::vector<LabelPointer>;

  const float& size = Settings::Window::size;

  // TODO: Move to settings
  const float rawFontSize = 24.0f;

public:
  ValueCategoryList()
  {
    generateDummyLabels();
    addAllLabels();
  }

  ~ValueCategoryList() override = default;

  void paint(juce::Graphics& _g) override { _g.fillAll(Colours::darkmagenta); }

  void resized() override
  {
    const auto fontSize = rawFontSize * size;

    auto bounds = getLocalBounds();
    for (auto& editor : labelList) {
      auto labelBounds = bounds.removeFromTop(fontSize);
      editor->setBounds(labelBounds);
    }
  }

  void setOptimalSize(const int width)
  {
    const auto fontSize = rawFontSize * size;
    const auto neededHeight = fontSize * labelList.size();
    setSize(width, neededHeight);
  }

protected:
  void generateDummyLabels()
  {
    for (int i = 0; i < 15; ++i) {
      const auto name = String("Category") + String(i);
      auto editor = std::make_unique<Lable>(name,
                                            fonts.light,
                                            24.0f,
                                            juce::Colours::white,
                                            juce::Justification::centredBottom);
      labelList.push_back(std::move(editor));
    }
  }
  void addAllLabels()
  {
    for (auto& editor : labelList) {
      addAndMakeVisible(*editor);
    }
  }

private:
  LabelList labelList;
  Fonts fonts;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueCategoryList)
};

} // namespace component
} // namespace gui
} // namespace dmt