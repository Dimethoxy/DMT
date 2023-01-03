/*
  ==============================================================================

    SynthVoice.h
    Created: 3 Jan 2023 4:48:44am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

namespace dmt {

struct SynthParameters
{
  float gain = 0.0f;
};

//==============================================================================
/*
        This class is used as a sound generating source.
        It has an oscillator that can generate sine, square and saw waveforms.
        Each object of this class has it's own ADSR envelope for gain
        modulation.
        It needs to be hosted inside a JUCE Synthesiser object.
        Each instance of this class can only play one note at the same time.
*/
class SynthVoice : public juce::SynthesiserVoice
{
public:
  bool canPlaySound(juce::SynthesiserSound* sound) override
  {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
  }

  void startNote(int midiNoteNumber,
                 float velocity,
                 juce::SynthesiserSound* sound,
                 int currentPitchWheelPosition) override
  {
  }

  void stopNote(float velocity, bool allowTailOff) override {}

  void controllerMoved(int controllerNumber, int newControllerValue) override {}

  void pitchWheelMoved(int newPitchWheelValue) override {}

  void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
  {
  }

  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
  }

private:
};

}