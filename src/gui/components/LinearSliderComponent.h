//==============================================================================
#pragma once
//==============================================================================
#include "../../utility/AppSettings.h"
#include "../../utility/Unit.h"
#include "../widgets/Label.h"
#include "../widgets/LinearSlider.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace components {
//==============================================================================
class LinearSliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = dmt::AppSettings;
  using Layout = dmt::AppSettings::Layout;
  using Fonts = Settings::Fonts;
  using Slider = Settings::Slider;
  using LinearSlider = dmt::gui::widgets::LinearSlider;
  using Type = LinearSlider::Type;
  using Orientation = LinearSlider::Orientation;
  using Label = dmt::gui::widgets::Label;
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
                        const Type type = Type::Positive)
    : orientation(LinearSlider::Orientation::Horizontal)
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
  }
  void paint(juce::Graphics& g)
  {
    auto bounds = getLocalBounds();
    // Draw bounds debug
    g.setColour(juce::Colours::green);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1);
  }
  void sliderValueChanged(juce::Slider*) { updateLabel(); }

  void setBoundsByPoints(juce::Point<int> newPrimaryPoint,
                         juce::Point<int> newSecondaryPoint)
  {
    primaryPoint = newPrimaryPoint.toFloat();
    secondaryPoint = newSecondaryPoint.toFloat();
    const int padding = (int)(rawPadding * size);
    const float primaryX = (float)primaryPoint.getX();
    const float primaryY = (float)primaryPoint.getY();
    const float secondaryX = (float)secondaryPoint.getX();
    const float secondaryY = (float)secondaryPoint.getY();

    const float rawMinWidth = 50.0f;
    const float minWidth = rawMinWidth * size;
    const float xDistance = secondaryX - primaryX;
    const float innerWidth = (minWidth > xDistance) ? minWidth : xDistance;

    const float rawMinHeight = 50.0f;
    const float minHeight = rawMinHeight * size;
    const float yDistance = secondaryY - primaryY;
    const float innerHeight = (minHeight > yDistance) ? minHeight : yDistance;

    const auto innerBounds =
      juce::Rectangle<float>(primaryX, primaryY, innerWidth, innerHeight);
    const auto centre = innerBounds.getCentre();

    const auto outerBounds = innerBounds.expanded(padding).withCentre(centre);
    setBounds(outerBounds.toNearestInt());
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
  juce::Point<float> primaryPoint;
  juce::Point<float> secondaryPoint;
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