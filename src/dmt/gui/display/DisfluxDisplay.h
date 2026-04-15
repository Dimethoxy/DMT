//==============================================================================
/* ██████╗ ██╗███╗   ███╗███████╗████████╗██╗  ██╗ ██████╗ ██╗  ██╗██╗   ██╗
 * ██╔══██╗██║████╗ ████║██╔════╝╚══██╔══╝██║  ██║██╔═══██╗╚██╗██╔╝╚██╗ ██╔╝
 * ██║  ██║██║██╔████╔██║█████╗     ██║   ███████║██║   ██║ ╚███╔╝  ╚████╔╝
 * ██║  ██║██║██║╚██╔╝██║██╔══╝     ██║   ██╔══██║██║   ██║ ██╔██╗   ╚██╔╝
 * ██████╔╝██║██║ ╚═╝ ██║███████╗   ██║   ██║  ██║╚██████╔╝██╔╝ ██╗   ██║
 * ╚═════╝ ╚═╝╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * Part of the Dimethoxy Library, primarily intended for Dimethoxy plugins.
 * External use is permitted but not recommended.
 * No support or compatibility guarantees are provided.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * Specialized display component inheriting from AbstractDisplay.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include "dsp/data/FifoAudioBuffer.h"
#include "gui/display/OscilloscopeDisplay.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace display {

//==============================================================================

/**
 * @class NeutrinoDisplay
 * @brief Specialized display component inheriting from
 * AbstractDisplay.
 */
class alignas(64) NeutrinoDisplay
  : public dmt::gui::display::OscilloscopeDisplay<float>
{
public:
  using FifoAudioBuffer = dmt::dsp::data::FifoAudioBuffer<float>;
  NeutrinoDisplay(FifoAudioBuffer& _fifoBuffer,
                  AudioProcessorValueTreeState& _apvts)
    : OscilloscopeDisplay(
        _fifoBuffer, // The data buffer
        _apvts,      // The processers value tree state
        true)        // This tells the display to not listen to the apvts
  {
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoDisplay)
};
} // namespace component
} // namespace gui
} // namespace dmt