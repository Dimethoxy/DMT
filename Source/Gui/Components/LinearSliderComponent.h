
#pragma once

#include "../../Utility/AppSettings.h"
#include "../../Utility/Unit.h"
#include "../Widgets/Label.h"
#include "../Widgets/LinearSlider.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace components {
class LinearSliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = dmt::AppSettings;
  using Fonts = Settings::Fonts;
  using Slider = Settings::Slider;
  const float& padding = Slider::padding;
  const float& titleFontSize = Slider::titleFontSize;
  const float& infoFontSize = Slider::infoFontSize;

public:
  LinearSliderComponent(juce::AudioProcessorValueTreeState& apvts,
                        juce::String name,
                        juce::String param,
                        dmt::InfoUnit::Type unitType)
    : sliderAttachment(apvts, param, slider)
    , titleLabel(name, titleFontSize, Fonts::regular)
    , infoLabel("Info Label", infoFontSize, Fonts::regular)
    , unitType(unitType)
  {
    slider.addListener(this);
    updateLabel();
    addAndMakeVisible(slider);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(infoLabel);
  }
  void resized()
  {
    float labelSize = 0.15f;
    titleLabel.setBoundsRelative(0.0f, 0.0f, 1.0f, labelSize);

    float sliderY = 0.0f;
    float sliderHeight = 1.0f;
    slider.setBoundsRelative(0.0f, sliderY, 1.0f, sliderHeight);

    float infoLabelY = 1.0f - labelSize;
    infoLabel.setBoundsRelative(0.0f, infoLabelY, 1.0f, labelSize);
  }
  void sliderValueChanged(juce::Slider* slider) { updateLabel(); }

private:
  dmt::gui::widgets::LinearSlider slider;
  juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
  dmt::gui::widgets::Label titleLabel;
  dmt::gui::widgets::Label infoLabel;
  dmt::InfoUnit::Type unitType;

  void updateLabel()
  {
    auto text = dmt::InfoUnit::getString(unitType, slider.getValue());
    infoLabel.setText(text, juce::NotificationType::dontSendNotification);
  }
};
} // namespace components
} // namespace gui
} // namespace dmt