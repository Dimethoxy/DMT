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
  using TreeAdapter = dmt::configuration::TreeAdapter;
  using CategoryList = std::vector<TreeAdapter::Category>;
  using CategoryCallback = std::function<void(TreeAdapter::Category&)>;

  const float& size = Settings::Window::size;

  // TODO: Move to settings
  const float rawFontSize = 16.0f;
  const Colour fontColour = juce::Colours::white;

public:
  ValueCategoryList(CategoryList& _categories,
                    CategoryCallback _onCategorySelected)
    : categories(_categories)
    , onCategorySelected(std::move(_onCategorySelected))
  {
    addCategories();
    onCategorySelected(categories[0]);
  }

  ~ValueCategoryList() override = default;

  void paint(juce::Graphics& _g) override {}

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
  void addCategories()
  {
    // Let's clear the current labels
    labelList.clear();
    // And generate new ones
    for (auto& category : categories) {
      const auto name = String(category.name) + String("Label");
      const auto text = String(category.name);
      auto label = std::make_unique<Lable>(name,
                                           fonts.medium,
                                           rawFontSize,
                                           fontColour,
                                           juce::Justification::centredLeft);
      label->setText(text);

      labelList.push_back(std::move(label));
    }
    addAllLabels();
  }

  void addAllLabels()
  {
    for (auto& label : labelList) {
      addAndMakeVisible(*label);
      label->addMouseListener(this, true);
    }
  }

  void mouseUp(const juce::MouseEvent& _event) override
  {
    const auto clickedLabel = dynamic_cast<Lable*>(_event.eventComponent);

    for (std::size_t i = 0; i < labelList.size(); ++i) {
      if (labelList[i].get() == clickedLabel) {
        onCategorySelected(categories[i]);
        break;
      }
    }
  }

private:
  LabelList labelList;
  Fonts fonts;

  //==============================================================================
  CategoryList& categories;
  CategoryCallback onCategorySelected;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueCategoryList)
};

} // namespace component
} // namespace gui
} // namespace dmt