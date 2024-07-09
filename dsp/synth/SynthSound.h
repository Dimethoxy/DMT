/*
  ==============================================================================

    SynthSound.h
    Created: 3 Jan 2023 4:49:12am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace dsp {
namespace synth {

//==============================================================================
/*
        This class is a implemantation of the juce::SynthesiserSound class.
        It is needed because the original juce class is a bastract class.
*/
class SynthSound : public juce::SynthesiserSound
{
public:
  bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
  bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

} // namespace synth
} // namespace dsp
} // namespace dmt