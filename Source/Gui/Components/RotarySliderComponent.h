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
//==============================================================================
class RotarySliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = dmt::AppSettings;
  using Layout = dmt::AppSettings::Layout;
  using Fonts = Settings::Fonts;
  using Slider = Settings::Slider;
  using RotarySLider = dmt::gui::widgets::RotarySlider;
  using Type = RotarySLider::Type;
  const float& size = Layout::size;
  const float& padding = Slider::padding;
  const juce::Colour& titleFontColour = Slider::titleFontColour;
  const juce::Colour& infoFontColour = Slider::infoFontColour;
  const float& titleFontSize = Slider::titleFontSize;
  const float& infoFontSize = Slider::infoFontSize;
  const float baseWidth = 120.0f;
  const float baseHeight = 150.0f;

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
                Fonts::light,
                infoFontSize,
                infoFontColour,
                juce::Justification::centredBottom)
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
    auto bounds = getLocalBounds();
    float rawSliderSize = 0.75f;
    float sliderSize = rawSliderSize * bounds.getHeight();
    slider.setBounds(bounds.removeFromTop(sliderSize));
    titleLabel.setBounds(bounds);
    infoLabel.setBounds(bounds);
  }
  void sliderValueChanged(juce::Slider* slider) { updateLabel(); }
  void setSizeAndCentre(juce::Point<int> centrePoint)
  {
    setSize(baseWidth * size, baseHeight * size);
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
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt