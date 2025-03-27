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
 * Specialized display component inheriting from AbstractDisplayComponent.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

#include "gui/component/AbstractDisplayComponent.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================

/**
 * @class DisfluxDisplayComponent
 * @brief Specialized display component inheriting from
 * AbstractDisplayComponent.
 */
class alignas(64) DisfluxDisplayComponent : public AbstractDisplayComponent
{
public:
  //============================================================================
  /**
   * @brief Constructor for DisfluxDisplayComponent.
   */
  inline DisfluxDisplayComponent() noexcept
    : AbstractDisplayComponent()
  {
  }

  //============================================================================
  /**
   * @brief Repaints the display.
   * @param _g Graphics context used for drawing.
   * @param _displayBounds Bounds of the display area.
   */
  forcedinline void paintDisplay(
    juce::Graphics& _g,
    const juce::Rectangle<int>& _displayBounds) const noexcept override
  {
    TRACER("DisfluxDisplayComponent::repaintDisplay");
  }

  //============================================================================
  /**
   * @brief Prepares the next frame.
   */
  forcedinline void prepareNextFrame() noexcept override
  {
    TRACER("DisfluxDisplayComponent::prepareNextFrame");
  }

  //============================================================================
  /**
   * @brief Resizes the display.
   */
  forcedinline void extendResized(
    const juce::Rectangle<int>& _displayBounds) noexcept override
  {
    TRACER("DisfluxDisplayComponent::extendResized");
  }

private:
  //============================================================================
  // Private members can be added here if needed
};
} // namespace component
} // namespace gui
} // namespace dmt