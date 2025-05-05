//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ███████ ██    ██   ███     ████
 * ██   ██ ██ ██  ██  ██ ██         ██    ██   ██ ██    ██  ██ ██     ██
 * ██████  ██ ██      ██ ███████    ██    ██   ██  ██████  ██   ██    ██
 *
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * This file is part of the Dimethoxy Library, a collection of essential
 * classes used across various Dimethoxy projects.
 * These files are primarily designed for internal use within our repositories.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * LinearSliderComponent provides a composite slider UI element with
 * optional SVG title, parameter binding, and context menu support.
 * Designed for real-time audio plugin GUIs.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
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

/**
 * @brief Composite slider component with optional SVG title and parameter
 * binding.
 *
 * @details
 * This component encapsulates a slider, title label (or SVG icon), and info
 * label. It supports parameter attachment, context menu integration, and
 * flexible layout. Designed for use in real-time audio plugin GUIs, with a
 * focus on performance and type safety. The component is non-copyable and
 * leak-detectable.
 */
class LinearSliderComponent
  : public juce::Component
  , public juce::Slider::Listener
  , public dmt::Scaleable<LinearSliderComponent>
  , public dmt::HostContextMenu<LinearSliderComponent>
{
  //==============================================================================
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
  // Members initialized as references to settings constants
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
  /**
   * @brief Constructs a LinearSliderComponent.
   *
   * @param _apvts Reference to the AudioProcessorValueTreeState for parameter
   * binding.
   * @param _text The title text for the slider.
   * @param _param The parameter ID to bind.
   * @param _unitType The unit type for value display.
   * @param _type The slider type (default: Positive).
   * @param _orientation The slider orientation (default: Horizontal).
   * @param _svgTitle If true, uses an SVG icon as the title.
   *
   * @details
   * The constructor sets up the slider, labels, parameter attachment, and
   * context menu. SVG title mode disables the text label and uses an icon
   * instead.
   */
  inline explicit LinearSliderComponent(
    juce::AudioProcessorValueTreeState& _apvts,
    const juce::String _text,
    const juce::String _param,
    const Unit::Type _unitType,
    const Type _type = Type::Positive,
    const Orientation _orientation = Orientation::Horizontal,
    const bool _svgTitle = false) noexcept
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
  /**
   * @brief Pointer to the attached parameter.
   *
   * @details
   * This is set during construction for context menu and value display.
   */
  RangedAudioParameter* parameter;

  //==============================================================================
  /**
   * @brief Shows the context menu for the attached parameter.
   *
   * @details
   * Uses HostContextMenu helper to display a context menu for the parameter.
   */
  inline void showContextMenuForSlider() noexcept
  {
    // Use HostContextMenu helper to show context menu for this parameter
    showContextMenu(parameter);
  }

  //==============================================================================
  /**
   * @brief Lays out the child components.
   *
   * @details
   * Arranges the slider, title label or icon, and info label according to
   * orientation. Padding and font sizes are scaled for DPI awareness.
   */
  inline void resized() override
  {
    TRACER("LinearSliderComponent::resized");
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;

    slider.setAlwaysOnTop(true);
    switch (orientation) {
      case Orientation::Horizontal: {
        const int32_t rawHorizontalSliderOffset =
          static_cast<int32_t>(1.0f * size);
        const juce::Point<int32_t> offset(0, rawHorizontalSliderOffset);
        const auto centre = bounds.getCentre() + offset;
        auto sliderBounds =
          bounds.reduced(static_cast<int32_t>(padding)).withCentre(centre);
        slider.setBounds(
          bounds.reduced(static_cast<int32_t>(padding)).withCentre(centre));
        auto titleLabelBounds = sliderBounds;
        const auto titleLabelHeight =
          static_cast<int32_t>(2 * titleFontSize * size);
        const auto titleLabelOffset = static_cast<int32_t>(4 * size);
        const auto titleSliderBounds =
          titleLabelBounds.removeFromTop(titleLabelHeight)
            .reduced(titleLabelOffset);
        titleLabel.setBounds(titleSliderBounds);
        auto infoLabelBounds = sliderBounds;
        const auto infoLabelHeight =
          static_cast<int32_t>(2 * infoFontSize * size);
        const auto infoLabelOffset = static_cast<int32_t>(9 * size);
        const auto infoSliderBounds =
          infoLabelBounds.removeFromBottom(infoLabelHeight)
            .reduced(infoLabelOffset);
        infoLabel.setBounds(infoSliderBounds);
        return;
      }
      case Orientation::Vertical: {
        titleLabel.setBounds(
          bounds.withTrimmedTop(static_cast<int32_t>(padding)));
        infoLabel.setBounds(
          bounds.withTrimmedBottom(static_cast<int32_t>(padding)));

        auto sliderBounds = bounds;
        sliderBounds.removeFromTop(
          static_cast<int32_t>(titleFontSize * size + padding));
        sliderBounds.removeFromBottom(
          static_cast<int32_t>(infoFontSize * size + padding));
        slider.setBounds(sliderBounds);
        return;
      }
    }
  }

  //==============================================================================
  /**
   * @brief Paints the component, including optional SVG icon.
   *
   * @param _g The graphics context.
   *
   * @details
   * Draws debug bounds if enabled. If SVG title mode is active, draws the icon
   * in the title area with appropriate padding.
   */
  inline void paint(juce::Graphics& _g) override
  {
    TRACER("LinearSliderComponent::paint");
    auto bounds = getLocalBounds();

    // Draw bounds debug
    _g.setColour(juce::Colours::green);
    if (Settings::debugBounds)
      _g.drawRect(bounds, 1);

    constexpr float baseSvgPadding = 2.0f;
    if (svgTitle) {
      juce::Rectangle<float> iconArea =
        bounds.removeFromTop(slider.getY()).toFloat();
      iconArea = iconArea.withY(iconArea.getY() + 6.0f * size);
      iconArea = iconArea.reduced((svgPadding + baseSvgPadding) * size);
      titleIcon->drawWithin(
        _g, iconArea, juce::RectanglePlacement::centred, 1.0f);
    }
  }

  //==============================================================================
  /**
   * @brief Called when the slider value changes.
   *
   * @param _slider The slider that changed (unused).
   * @details Updates the info label to reflect the new value.
   */
  inline void sliderValueChanged(juce::Slider* /*_slider*/) override
  {
    TRACER("LinearSliderComponent::sliderValueChanged");
    updateLabel();
  }

  //==============================================================================
  /**
   * @brief Sets the bounds of the component based on two points.
   *
   * @param _primaryPoint The first anchor point.
   * @param _secondaryPoint The second anchor point.
   *
   * @details
   * Used for interactive placement or resizing. Ensures minimum size and
   * orientation-specific layout.
   */
  inline void setBoundsByPoints(juce::Point<int32_t> _primaryPoint,
                                juce::Point<int32_t> _secondaryPoint) noexcept
  {
    TRACER("LinearSliderComponent::setBoundsByPoints");
    const float padding = 2.0f * rawPadding * size;
    const float minHeight = 50 * size;
    const float minWidth = 40 * size;

    const auto centre = (_primaryPoint + _secondaryPoint).toFloat() / 2.0f;
    const int32_t pointDistance =
      _primaryPoint.getDistanceFrom(_secondaryPoint);

    switch (orientation) {
      case Orientation::Horizontal: {
        setBounds(juce::Rectangle<int32_t>()
                    .withSize(pointDistance, static_cast<int32_t>(minHeight))
                    .expanded(static_cast<int32_t>(padding))
                    .withCentre(centre.toInt()));
        return;
      }
      case Orientation::Vertical: {
        setBounds(juce::Rectangle<int32_t>()
                    .withSize(static_cast<int32_t>(minWidth), pointDistance)
                    .expanded(static_cast<int32_t>(padding))
                    .withCentre(centre.toInt()));
        return;
      }
    }
  }

  //==============================================================================
  /**
   * @brief Returns a reference to the internal slider.
   *
   * @return Reference to the LinearSlider.
   *
   * @details
   * Allows external code to access or customize the slider directly.
   */
  [[nodiscard]] inline juce::Slider& getSlider() noexcept
  {
    TRACER("LinearSliderComponent::getSlider");
    return slider;
  }

  //==============================================================================
protected:
  //==============================================================================
  /**
   * @brief Updates the info label with the current slider value.
   *
   * @details
   * Converts the slider value to a string using the specified unit type.
   * Forces a repaint of the info label for immediate feedback.
   */
  inline void updateLabel() noexcept
  {
    TRACER("LinearSliderComponent::updateLabel");
    auto text =
      Unit::getString(unitType, static_cast<float>(slider.getValue()));
    infoLabel.setText(text);
    infoLabel.repaint();
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  LinearSlider slider;
  SliderAttachment sliderAttachment;
  Label titleLabel;
  Label infoLabel;
  Unit::Type unitType;
  Orientation orientation;
  Fonts fonts;
  const bool svgTitle;
  const float svgPadding;

  //==============================================================================
  // Other members
  std::unique_ptr<juce::Drawable> titleIcon;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinearSliderComponent)
};
//==============================================================================
} // namespace component
} // namespace gui
} // namespace dmt