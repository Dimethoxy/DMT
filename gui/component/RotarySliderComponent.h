//==============================================================================
#pragma once
//==============================================================================
#include "gui/widget/Label.h"
#include "gui/widget/RotarySlider.h"
#include "utility/Fonts.h"
#include "utility/Icon.h"
#include "utility/Settings.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class RotarySliderComponent
  : public juce::Component
  , public juce::Slider::Listener
  , public dmt::Scaleable<RotarySliderComponent>
{
  using RotarySlider = dmt::gui::widget::RotarySlider;
  using Type = RotarySlider::Type;
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  using Unit = dmt::utility::Unit;
  using Label = dmt::gui::widget::Label;
  using Fonts = dmt::utility::Fonts;
  using Settings = dmt::Settings;
  using Layout = dmt::Settings::Window;
  using Slider = Settings::Slider;

  //============================================================================
  const float& baseWidth = Slider::baseWidth;
  const float& baseHeight = Slider::baseHeight;
  const float& rawSliderSize = Slider::sliderSize;
  const float& rawLabelsSize = Slider::labelsSize;
  const float& rawPadding = Slider::padding;
  const juce::Colour& titleFontColour = Slider::titleFontColour;
  const juce::Colour& infoFontColour = Slider::infoFontColour;
  const float& titleFontSize = Slider::titleFontSize;
  const float& infoFontSize = Slider::infoFontSize;

public:
  RotarySliderComponent(juce::AudioProcessorValueTreeState& apvts,
                        const juce::String text,
                        const juce::String param,
                        const Unit::Type unitType,
                        const Type type = Type::Positive)
    : slider(type)
    , sliderAttachment(apvts, param, slider)
    , titleLabel(text, fonts.medium, titleFontSize, titleFontColour)
    , infoLabel(juce::String("Info Label"),
                fonts.light,
                infoFontSize,
                infoFontColour,
                juce::Justification::centredBottom)
    , unitType(unitType)
  {
    TRACER("RotarySliderComponent::RotarySliderComponent");
    slider.addListener(this);
    updateLabel();
    addAndMakeVisible(slider);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(infoLabel);
  }
  void resized()
  {
    TRACER("RotarySliderComponent::resized");
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;

    auto sliderBounds = bounds;
    float sliderSize = rawSliderSize * sliderBounds.getHeight();
    slider.setBounds(sliderBounds.removeFromTop((int)sliderSize));

    int labelPadding = (int)(padding * 0.5f);
    auto labelsBounds = bounds.reduced(labelPadding);
    float labelsSize = rawLabelsSize * labelsBounds.getHeight();
    labelsBounds = labelsBounds.removeFromBottom((int)labelsSize);

    titleLabel.setBounds(labelsBounds);
    infoLabel.setBounds(labelsBounds);
  }
  void paint(juce::Graphics& g)
  {
    TRACER("RotarySliderComponent::paint");
    auto bounds = getLocalBounds();
    // Draw bounds debug
    g.setColour(juce::Colours::green);
    if (Settings::debugBounds) {
      g.drawRect(bounds, 1);
      // draw center
      g.setColour(juce::Colours::green);
      g.drawEllipse(bounds.getCentreX(), bounds.getCentreY(), 5, 5, 1);
    }
  }
  void sliderValueChanged(juce::Slider*)
  {
    TRACER("RotarySliderComponent::sliderValueChanged");
    updateLabel();
  }
  void setSizeAndCentre(juce::Point<int> centrePoint)
  {
    TRACER("RotarySliderComponent::setSizeAndCentre");
    int width = (int)(baseWidth * size);
    int height = (int)(baseHeight * size);
    setSize(width, height);
    setCentrePosition(centrePoint.getX(), centrePoint.getY());
  }

protected:
  void updateLabel()
  {
    TRACER("RotarySliderComponent::updateLabel");
    auto text = Unit::getString(unitType, (float)slider.getValue());
    infoLabel.setText(text);
    infoLabel.repaint();
  }

private:
  RotarySlider slider;
  SliderAttachment sliderAttachment;
  Label titleLabel;
  Label infoLabel;
  Unit::Type unitType;
  Fonts fonts;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySliderComponent)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt