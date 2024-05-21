//==============================================================================
#pragma once
//==============================================================================
#include "gui/widget/Label.h"
#include "gui/widget/LinearSlider.h"
#include "utility/Settings.h"
#include "utility/Unit.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class LinearSliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = dmt::Settings;
  using Layout = Settings::Layout;
  using Fonts = Settings::Fonts;
  using Slider = Settings::Slider;
  using LinearSlider = dmt::gui::widget::LinearSlider;
  using Type = LinearSlider::Type;
  using Orientation = LinearSlider::Orientation;
  using Label = dmt::gui::widget::Label;
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  const float& size = Layout::size;
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
  LinearSliderComponent(juce::AudioProcessorValueTreeState& apvts,
                        const juce::String text,
                        const juce::String param,
                        const dmt::InfoUnit::Type unitType,
                        const Type type = Type::Positive,
                        const Orientation orientation = Orientation::Horizontal)
    : orientation(orientation)
    , slider(type, orientation)
    , sliderAttachment(apvts, param, slider)
    , titleLabel(text, Fonts::medium, titleFontSize, titleFontColour)
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
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;

    titleLabel.setBounds(bounds.reduced(padding));
    infoLabel.setBounds(bounds.reduced(padding));

    slider.setAlwaysOnTop(true);
    switch (orientation) {
      case Orientation::Horizontal: {
        const int rawHorizontalSliderOffset = (int)(1.0f * size);
        const juce::Point<int> offset(0, rawHorizontalSliderOffset);
        const auto centre = bounds.getCentre() + offset;
        slider.setBounds(bounds.reduced((int)padding).withCentre(centre));
        return;
      }
      case Orientation::Vertical: {
        auto sliderBounds = bounds.reduced(padding);
        sliderBounds.removeFromTop(titleFontSize * size);
        sliderBounds.removeFromBottom(infoFontSize * size);
        slider.setBounds(sliderBounds);
        return;
      }
    }
  }
  void paint(juce::Graphics& g)
  {
    const auto bounds = getLocalBounds();

    // Draw bounds debug
    g.setColour(juce::Colours::green);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1);
  }
  void sliderValueChanged(juce::Slider*) { updateLabel(); }

  void setBoundsByPoints(juce::Point<int> primaryPoint,
                         juce::Point<int> secondaryPoint)
  {
    const float padding = 2.0f * rawPadding * size;
    const float minHeight = 34 * size;
    const float minWidth = 40 * size;

    const auto centre = (primaryPoint + secondaryPoint).toFloat() / 2.0f;
    const int pointDistance = primaryPoint.getDistanceFrom(secondaryPoint);

    switch (orientation) {
      case Orientation::Horizontal: {
        setBounds(juce::Rectangle<int>()
                    .withSize(pointDistance, minHeight)
                    .expanded(padding)
                    .withCentre(centre.toInt()));
        return;
      }
      case Orientation::Vertical: {
        setBounds(juce::Rectangle<int>()
                    .withSize(minWidth, pointDistance)
                    .expanded(padding)
                    .withCentre(centre.toInt()));
        return;
      }
    }
  }

protected:
  void updateLabel()
  {
    auto text = dmt::InfoUnit::getString(unitType, (float)slider.getValue());
    infoLabel.setText(text);
    infoLabel.repaint();
  }

private:
  LinearSlider::Orientation orientation;
  dmt::InfoUnit::Type unitType;
  LinearSlider slider;
  SliderAttachment sliderAttachment;
  Label titleLabel;
  Label infoLabel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSliderComponent)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt