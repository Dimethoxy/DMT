//==============================================================================
#pragma once
//==============================================================================
#include "../../Utility/AppSettings.h"
#include "../../Utility/Unit.h"
#include "../Widgets/Label.h"
#include "../Widgets/RotarySlider.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace components {
class RotarySliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = dmt::AppSettings;
  using Fonts = Settings::Fonts;
  using Slider = Settings::Slider;
  using RotarySLider = dmt::gui::widgets::RotarySlider;
  using Type = RotarySLider::Type;
  const float& padding = Slider::padding;
  const juce::Colour& titleFontColour = Slider::titleFontColour;
  const juce::Colour& infoFontColour = Slider::infoFontColour;
  const float& titleFontSize = Slider::titleFontSize;
  const float& infoFontSize = Slider::infoFontSize;

public:
  RotarySliderComponent(juce::AudioProcessorValueTreeState& apvts,
                        const juce::String text,
                        const juce::String param,
                        const dmt::InfoUnit::Type unitType,
                        const Type type = Type::Positive)
    : slider(type)
    , sliderAttachment(apvts, param, slider)
    , titleLabel(text, Fonts::regular, titleFontSize, titleFontColour)
    , infoLabel(juce::String("Info Label"),
                Fonts::regular,
                infoFontSize,
                infoFontColour)
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
  void setSizeAndCentre(juce::Point<int> centrePoint)
  {
    setSize(120, 150);
    setCentrePosition(centrePoint);
  }

private:
  dmt::gui::widgets::RotarySlider slider;
  juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
  dmt::gui::widgets::Label titleLabel;
  dmt::gui::widgets::Label infoLabel;
  dmt::InfoUnit::Type unitType;

  void updateLabel()
  {
    auto text = dmt::InfoUnit::getString(unitType, slider.getValue());
    infoLabel.setText(text);
  }
};
} // namespace components
} // namespace gui
} // namespace dmt