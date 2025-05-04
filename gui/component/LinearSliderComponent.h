//==============================================================================
#pragma once
//==============================================================================
#include "gui/widget/Label.h"
#include "gui/widget/LinearSlider.h"
#include "utility/Fonts.h"
#include "utility/HostContextMenu.h"
#include "utility/Icon.h"
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
  , public dmt::Scaleable<LinearSliderComponent>
  , public dmt::HostContextMenu<LinearSliderComponent>
{
  using LinearSlider = dmt::gui::widget::LinearSlider;
  using Type = LinearSlider::Type;
  using Orientation = LinearSlider::Orientation;
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  using Unit = dmt::utility::Unit;
  using Label = dmt::gui::widget::Label;
  using Fonts = dmt::utility::Fonts;
  using Settings = dmt::Settings;
  using Layout = Settings::Window;
  using Slider = Settings::Slider;
  using RangedAudioParameter = juce::RangedAudioParameter;

  //==============================================================================
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
  //==============================================================================
  LinearSliderComponent(
    juce::AudioProcessorValueTreeState& _apvts,
    const juce::String _text,
    const juce::String _param,
    const Unit::Type _unitType,
    const Type _type = Type::Positive,
    const Orientation _orientation = Orientation::Horizontal,
    const bool _svgTitle = false)
    : orientation(_orientation)
    , slider(_type, _orientation)
    , sliderAttachment(_apvts, _param, slider)
    , titleLabel(_text, fonts.medium, titleFontSize, titleFontColour)
    , infoLabel(juce::String("Info Label"),
                fonts.light,
                infoFontSize,
                infoFontColour,
                juce::Justification::centredBottom)
    , unitType(_unitType)
    , svgTitle(_svgTitle)
    , svgPadding(dmt::icons::getPadding(_param))
  {
    TRACER("LinearSliderComponent::LinearSliderComponent");
    slider.addListener(this);
    updateLabel();
    addAndMakeVisible(slider);
    addAndMakeVisible(infoLabel);

    if (!_svgTitle)
      addAndMakeVisible(titleLabel);

    if (_svgTitle) {
      titleIcon = dmt::icons::getIcon(_param);
    }

    // Let's find the pointer to the parameter in the apvts
    parameter = _apvts.getParameter(_param);

    // Set up context menu callback
    slider.onContextMenuRequested = [this]() { showContextMenuForSlider(); };
  }
  //==============================================================================
  void resized()
  {
    TRACER("LinearSliderComponent::resized");
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;

    slider.setAlwaysOnTop(true);
    switch (orientation) {
      case Orientation::Horizontal: {
        const int rawHorizontalSliderOffset = (int)(1.0f * size);
        const juce::Point<int> offset(0, rawHorizontalSliderOffset);
        const auto centre = bounds.getCentre() + offset;
        auto const sliderBounds =
          bounds.reduced((int)padding).withCentre(centre);
        slider.setBounds(bounds.reduced((int)padding).withCentre(centre));
        auto titleLabelBounds = sliderBounds;
        const auto titleLabelHeight = 2 * titleFontSize * size;
        const auto titleLabelOffset = 4 * size;
        const auto titleSliderBounds =
          titleLabelBounds.removeFromTop(titleLabelHeight)
            .reduced(titleLabelOffset);
        titleLabel.setBounds(titleSliderBounds);
        auto infoLabelBounds = sliderBounds;
        const auto infoLabelHeight = 2 * infoFontSize * size;
        const auto infoLabelOffset = 9 * size;
        const auto infoSliderBounds =
          infoLabelBounds.removeFromBottom(infoLabelHeight)
            .reduced(infoLabelOffset);
        infoLabel.setBounds(infoSliderBounds);
        return;
      }
      case Orientation::Vertical: {
        titleLabel.setBounds(bounds.withTrimmedTop(padding));
        infoLabel.setBounds(bounds.withTrimmedBottom(padding));

        auto sliderBounds = bounds;
        sliderBounds.removeFromTop(titleFontSize * size + padding);
        sliderBounds.removeFromBottom(infoFontSize * size + padding);
        slider.setBounds(sliderBounds);
        return;
      }
    }
  }
  //==============================================================================
  void paint(juce::Graphics& g)
  {
    TRACER("LinearSliderComponent::paint");
    auto bounds = getLocalBounds();

    // Draw bounds debug
    g.setColour(juce::Colours::green);
    if (Settings::debugBounds)
      g.drawRect(bounds, 1);

    float baseSvgPadding = 2.0f;
    if (svgTitle) {
      juce::Rectangle<float> iconArea =
        bounds.removeFromTop(slider.getY()).toFloat();
      iconArea = iconArea.withY(iconArea.getY() + 6.0f * size);
      iconArea = iconArea.reduced((svgPadding + baseSvgPadding) * size);
      titleIcon->drawWithin(
        g, iconArea, juce::RectanglePlacement::centred, 1.0f);
    }
  }
  //==============================================================================
  void sliderValueChanged(juce::Slider*)
  {
    TRACER("LinearSliderComponent::sliderValueChanged");
    updateLabel();
  }
  //==============================================================================
  void setBoundsByPoints(juce::Point<int> primaryPoint,
                         juce::Point<int> secondaryPoint)
  {
    TRACER("LinearSliderComponent::setBoundsByPoints");
    const float padding = 2.0f * rawPadding * size;
    const float minHeight = 50 * size;
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
  //==============================================================================
  juce::Slider& getSlider()
  {
    TRACER("LinearSliderComponent::getSlider");
    return slider;
  }
  //==============================================================================
protected:
  void updateLabel()
  {
    TRACER("LinearSliderComponent::updateLabel");
    auto text = Unit::getString(unitType, (float)slider.getValue());
    infoLabel.setText(text);
    infoLabel.repaint();
  }
  //==============================================================================

  void showContextMenuForSlider()
  {
    // Use HostContextMenu helper to show context menu for this parameter
    showContextMenu(parameter);
  }

private:
  Fonts fonts;
  Orientation orientation;
  Unit::Type unitType;
  LinearSlider slider;
  SliderAttachment sliderAttachment;
  Label titleLabel;
  Label infoLabel;
  const bool svgTitle;
  std::unique_ptr<juce::Drawable> titleIcon;
  RangedAudioParameter* parameter;
  const float svgPadding;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSliderComponent)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt