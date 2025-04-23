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
  const float rawFontSize = 16.0f;
  const Colour fontColour = juce::Colours::white;

public:
  ValueCategoryList()
  {
    generateDummyLabels();
    addAllLabels();
  }

  ~ValueCategoryList() override = default;

  void paint(juce::Graphics& _g) override { _g.fillAll(Colours::darkgrey); }

  void resized() override
  {
    const auto fontSize = rawFontSize * size;

    auto bounds = getLocalBounds();
    for (auto& label : labelList) {
      auto labelBounds = bounds.removeFromTop(fontSize);
      label->setBounds(labelBounds);
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
      const auto name = String("Category ") + String(i);
      auto label = std::make_unique<Lable>(name,
                                           fonts.medium,
                                           rawFontSize,
                                           fontColour,
                                           juce::Justification::centredBottom);
      label->setText(name);
      labelList.push_back(std::move(label));
    }
  }
  void addAllLabels()
  {
    for (auto& label : labelList) {
      addAndMakeVisible(*label);
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