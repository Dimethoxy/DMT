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
 * RotarySliderComponent provides a composite widget for rotary slider controls,
 * including parameter binding, context menu, and value display. Designed for
 * real-time audio plugin GUIs with strict performance and type safety.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once
//==============================================================================
#include "gui/widget/Label.h"
#include "gui/widget/RotarySlider.h"
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
 * @brief Composite rotary slider component with parameter binding and context
 * menu.
 *
 * @details
 * This class encapsulates a rotary slider, a title label, and an info label
 * displaying the current value with units. It binds to a parameter in a
 * JUCE AudioProcessorValueTreeState for real-time automation and supports
 * host context menus for parameter management.
 *
 * Intended for use in audio plugin GUIs where real-time safety and
 * responsiveness are critical. All layout and painting are optimized for
 * performance and clarity.
 */
class RotarySliderComponent
  : public juce::Component
  , public juce::Slider::Listener
  , public dmt::Scaleable<RotarySliderComponent>
  , public dmt::HostContextMenu<RotarySliderComponent>
{
  //==============================================================================
  using RotarySlider = dmt::gui::widget::RotarySlider;
  using Type = RotarySlider::Type;
  using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
  using Unit = dmt::utility::Unit;
  using Label = dmt::gui::widget::Label;
  using Fonts = dmt::utility::Fonts;
  using Settings = dmt::Settings;
  using Layout = dmt::Settings::Window;
  using Slider = Settings::Slider;
  using RangedAudioParameter = juce::RangedAudioParameter;

  //==============================================================================
  // Constant references for layout and appearance
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
   * @brief Constructs a RotarySliderComponent with parameter binding.
   *
   * @param _apvts Reference to the AudioProcessorValueTreeState for parameter
   * binding.
   * @param _text The label text for the slider.
   * @param _param The parameter ID to bind the slider to.
   * @param _unitType The unit type for value display.
   * @param _type The rotary slider type (default: Positive).
   *
   * @details
   * The constructor sets up the slider, attaches it to the parameter,
   * initializes labels, and configures the context menu callback.
   * All layout and font settings are derived from the Settings singleton.
   */
  inline explicit RotarySliderComponent(
    juce::AudioProcessorValueTreeState& _apvts,
    const juce::String _text,
    const juce::String _param,
    const Unit::Type _unitType,
    const Type _type = Type::Positive) noexcept
    : slider(_type)
    , sliderAttachment(_apvts, _param, slider)
    , titleLabel(_text, fonts.medium, titleFontSize, titleFontColour)
    , infoLabel(juce::String("Info Label"),
                fonts.light,
                infoFontSize,
                infoFontColour,
                juce::Justification::centredBottom)
    , unitType(_unitType)
  {
    TRACER("RotarySliderComponent::RotarySliderComponent");
    slider.addListener(this);
    updateLabel();
    addAndMakeVisible(slider);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(infoLabel);

    // Let's find the pointer to the parameter in the apvts
    parameter = _apvts.getParameter(_param);

    // Set up context menu callback for host automation and parameter actions
    slider.onContextMenuRequested = [this]() { showContextMenuForSlider(); };
  }

  //==============================================================================
  /**
   * @brief Pointer to the attached parameter in the APVTS.
   *
   * @details
   * Used for context menu and parameter automation. Set in the constructor.
   */
  RangedAudioParameter* parameter = nullptr;

  //==============================================================================
  /**
   * @brief Shows the host context menu for the attached parameter.
   *
   * @details
   * Invokes the HostContextMenu helper to display parameter automation and
   * management options. This is typically triggered by right-clicking the
   * slider.
   */
  inline void showContextMenuForSlider() noexcept
  {
    // Use HostContextMenu helper to show context menu for this parameter
    showContextMenu(parameter);
  }

  //==============================================================================
  /**
   * @brief Handles component resizing and lays out child widgets.
   *
   * @details
   * Calculates bounds for the slider and labels based on current scaling and
   * padding. Ensures all subcomponents are correctly positioned and sized.
   */
  inline void resized() override
  {
    TRACER("RotarySliderComponent::resized");
    const auto bounds = getLocalBounds();
    const auto padding = rawPadding * size;

    auto sliderBounds = bounds;
    const auto sliderSize =
      static_cast<int>(rawSliderSize * sliderBounds.getHeight());
    slider.setBounds(sliderBounds.removeFromTop(sliderSize));

    const int labelPadding = static_cast<int>(padding * 0.5f);
    auto labelsBounds = bounds.reduced(labelPadding);
    const auto labelsSize =
      static_cast<int>(rawLabelsSize * labelsBounds.getHeight());
    labelsBounds = labelsBounds.removeFromBottom(labelsSize);

    titleLabel.setBounds(labelsBounds);
    infoLabel.setBounds(labelsBounds);
  }

  //==============================================================================
  /**
   * @brief Paints debug bounds and center marker if enabled.
   *
   * @param _g The graphics context.
   *
   * @details
   * Only draws debug outlines and center marker if Settings::debugBounds is
   * true. All other painting is handled by subcomponents for maximum
   * flexibility.
   */
  inline void paint(juce::Graphics& _g) override
  {
    TRACER("RotarySliderComponent::paint");
    auto bounds = getLocalBounds();
    // Draw bounds debug
    _g.setColour(juce::Colours::green);
    if (Settings::debugBounds) {
      _g.drawRect(bounds, 1);
      // draw center
      _g.setColour(juce::Colours::green);
      _g.drawEllipse(bounds.getCentreX(), bounds.getCentreY(), 5, 5, 1);
    }
  }

  //==============================================================================
  /**
   * @brief Updates the info label when the slider value changes.
   *
   * @param _slider The slider that changed (unused).
   * @details This method is called by the slider listener.
   * It updates the info label to reflect the new value,
   * formatted with the appropriate unit.
   */
  inline void sliderValueChanged(juce::Slider* /*_slider*/) override
  {
    TRACER("RotarySliderComponent::sliderValueChanged");
    updateLabel();
  }

  //==============================================================================
  /**
   * @brief Sets the size and center position of the component.
   *
   * @param _centrePoint The point to center the component on.
   *
   * @details
   * Calculates the size based on scaling and centers the component at the
   * specified point. Used for dynamic layout in parent containers.
   */
  inline void setSizeAndCentre(juce::Point<int> _centrePoint) noexcept
  {
    TRACER("RotarySliderComponent::setSizeAndCentre");
    const int width = static_cast<int>(baseWidth * size);
    const int height = static_cast<int>(baseHeight * size);
    setSize(width, height);
    setCentrePosition(_centrePoint.getX(), _centrePoint.getY());
  }

protected:
  //==============================================================================
  /**
   * @brief Updates the info label with the current slider value and unit.
   *
   * @details
   * Formats the slider value using the Unit helper and updates the info label.
   * Forces a repaint to ensure immediate visual feedback.
   */
  inline void updateLabel() noexcept
  {
    TRACER("RotarySliderComponent::updateLabel");
    const auto text =
      Unit::getString(unitType, static_cast<float>(slider.getValue()));
    infoLabel.setText(text);
    infoLabel.repaint();
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  RotarySlider slider;
  SliderAttachment sliderAttachment;
  Label titleLabel;
  Label infoLabel;
  Unit::Type unitType;
  Fonts fonts;

  //==============================================================================
  // Other members

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotarySliderComponent)
};
//==============================================================================
} // namespace component
} // namespace gui
} // namespace dmt