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

#include "dsp/data/FifoAudioBuffer.h"
#include "gui/component/OscilloscopeComponent.h"
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
class alignas(64) DisfluxDisplayComponent
  : public dmt::gui::component::OscilloscopeComponent<float>
{
public:
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<float>;
  DisfluxDisplayComponent(FifoAudioBuffer& _fifoBuffer,
                          AudioProcessorValueTreeState& _apvts)
    : OscilloscopeComponent(_fifoBuffer, _apvts)
  {
  }
};
} // namespace component
} // namespace gui
} // namespace dmt