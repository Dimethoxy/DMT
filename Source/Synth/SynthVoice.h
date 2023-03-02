/*
  ==============================================================================

    SynthVoice.h
    Created: 3 Jan 2023 4:48:44am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../Envelope/AdhEnvelope.h"
#include "../Synth/AnalogOscillator.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {

struct SynthParameters
{
  float gain = 0.0f;
  float duration = 0.0f;
  float modDecay = 0.3f;
  float modDepth = 5000.0f;
  float modScew = 70.0f;
};

//==============================================================================

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
    dmt::AhdEnvelope::Parameters params;
    params.attack = 0.005f;
    params.hold = 0.08f;
    params.decay = 0.5f;
    params.decayScew = 0;
    gainEnvelope.setParameters(params);
    params.attack = 0.0f;
    params.hold = 0.0f;
    params.decay = synthParams.modDecay;
    params.decayScew = synthParams.modScew;
    pitchEnvelope.setParameters(params);
    baseFreq = midiNoteNumber;
    osc.resetPhase();
    gainEnvelope.noteOn();
    pitchEnvelope.noteOn();
  }

  void setParams(SynthParameters param) { this->synthParams = param; };

  void stopNote(float velocity, bool allowTailOff) override
  {
    // if (!allowTailOff || !gainEnvelope.isActive())
    //   clearCurrentNote();
  }
  void controllerMoved(int controllerNumber, int newControllerValue) override {}

  void pitchWheelMoved(int newPitchWheelValue) override {}

  void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
  {
    juce::dsp::ProcessSpec spec;
    gainEnvelope.setSampleRate(sampleRate);
    pitchEnvelope.setSampleRate(sampleRate);
    osc.setSampleRate(sampleRate);
    isPrepared = true;
  }

  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
    // Exit if the voice is not playing
    if (!isVoiceActive() || !isPrepared)
      return;

    // Render Oscillator
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = startSample; sample < (numSamples + startSample);
         sample++) {
      // Calculate frquency
      float freqModDepth = baseFreq + synthParams.modDepth;
      float envelopeSample = pitchEnvelope.getNextSample();
      float newFreq = juce::mapToLog10(envelopeSample, baseFreq, freqModDepth);
      osc.setFrequency(std::clamp(newFreq, 20.0f, 20000.0f));
      // Calculate gain
      auto oscGain = gainEnvelope.getNextSample();
      auto currentSample = osc.getNextSample() * oscGain;

      // Write final sample
      leftChannel[sample] = currentSample;
      rightChannel[sample] = currentSample;
    }
  }

private:
  SynthParameters synthParams;
  dmt::AnalogOscillator osc;
  dmt::AhdEnvelope gainEnvelope;
  dmt::AhdEnvelope pitchEnvelope;
  float baseFreq = 0.0f;
  float pitchDepth = 0.7;
  bool isPrepared = false;
};

}