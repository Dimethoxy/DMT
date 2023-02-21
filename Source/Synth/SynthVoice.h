/*
  ==============================================================================

    SynthVoice.h
    Created: 3 Jan 2023 4:48:44am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../Envelope/AdhEnvelope.h"
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
    note = midiNoteNumber;
    pitchEnvelope.noteOn();
    gainEnvelope.noteOn();
  }

  void stopNote(float velocity, bool allowTailOff) override
  {
    gainEnvelope.noteOff();
    // pitchEnvelope.noteOff();

    if (!allowTailOff || !gainEnvelope.isActive())
      clearCurrentNote();
  }

  void controllerMoved(int controllerNumber, int newControllerValue) override {}

  void pitchWheelMoved(int newPitchWheelValue) override {}

  void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
  {
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;

    osc.prepare(spec);
    gain.prepare(spec);
    pitchEnvelope.setSampleRate(sampleRate);
    gainEnvelope.setSampleRate(sampleRate);

    dmt::AhdEnvelope::Parameters pitchEnvelopeParams;
    pitchEnvelopeParams.attack = 1.0f;
    pitchEnvelopeParams.hold = 1.0f;
    pitchEnvelopeParams.decay = 1.0;

    juce::ADSR::Parameters gainEnvelopeParams;
    gainEnvelopeParams.attack = 0.1f;
    gainEnvelopeParams.decay = 1.0f;
    gainEnvelopeParams.sustain = 0.5f;
    gainEnvelopeParams.release = 0.2f;
    gainEnvelope.setParameters(gainEnvelopeParams);

    osc.setFrequency(60.0f);
    gain.setGainLinear(1.0f);
  }

  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = 0; sample < outputBuffer.getNumSamples(); sample++) {
      osc.setFrequency(note);
      auto frequencyModulation = pitchEnvelope.getNextSample() + 1.0f;
      /*auto newFrequency =
        osc.getFrequency() +
        frequencyModulation * (2000.0f - osc.getFrequency() * pitchDepth);
      std::clamp(newFrequency, 20.0f, 20000.0f);
      osc.setFrequency(newFrequency);*/
      auto currentSample =
        osc.processSample(0.0f) * pitchEnvelope.getNextSample();
      leftChannel[sample] = currentSample;
      rightChannel[sample] = currentSample;
    }
  }

private:
  float wave(float x)
  {
    const auto pi = juce::MathConstants<float>::pi;
    auto signBit = std::signbit(x) ? -1 : 1;
    auto result = x * signBit / pi - pi;
    return result;
  }
  juce::dsp::Oscillator<float> osc{ [&](float x) { return wave(x); } };
  juce::dsp::Gain<float> gain;
  dmt::AhdEnvelope pitchEnvelope;
  juce::ADSR gainEnvelope;
  int note = 0;
  float pitchDepth = 0.7;
};

}