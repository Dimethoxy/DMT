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

//==============================================================================
// SynthParameters
//==============================================================================
struct SynthParameters
{
  float gain = 0.0f;
  float duration = 0.0f;
  float modDecay = 0.3f;
  float modDepth = 5000.0f;
  float modScew = 70.0f;
};
//==============================================================================
// SynthVoice
//==============================================================================
class SynthVoice : public juce::SynthesiserVoice
{
public:
  //============================================================================
  bool canPlaySound(juce::SynthesiserSound* sound) override
  {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
  }
  void controllerMoved(int controllerNumber, int newControllerValue) override {}
  void pitchWheelMoved(int newPitchWheelValue) override {}
  //============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
  {
    // Exit if the sample rate is invalid
    if (sampleRate <= 0)
      return;

    // Prepare envelope
    gainEnvelope.setSampleRate(sampleRate);
    pitchEnvelope.setSampleRate(sampleRate);

    // Prepare oscillator
    osc.setSampleRate(sampleRate);

    // Set state as prepared
    isPrepared = true;
  }
  //============================================================================
  void startNote(int midiNoteNumber,
                 float velocity,
                 juce::SynthesiserSound* sound,
                 int currentPitchWheelPosition) override
  {
    // Set oscillator frequency
    osc.setPhase(0.0f);
    baseFreq = midiNoteNumber;

    // Start envelopes
    setEnvelopes();
    gainEnvelope.noteOn();
    pitchEnvelope.noteOn();
  }
  void stopNote(float velocity, bool allowTailOff) override {}
  //============================================================================
  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
    // Exit if the voice is not playing or the voice is not prepared
    if (!isVoiceActive() || !isPrepared)
      return;

    // Render oscillator
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = startSample; sample < (numSamples + startSample);
         sample++) {
      // Calculate frquency
      float freqModDepth = baseFreq + synthParams.modDepth;
      float envelopeSample = pitchEnvelope.getNextSample();
      float newFreq = juce::mapToLog10(envelopeSample, baseFreq, freqModDepth);
      osc.setFrequency(std::clamp(newFreq, 20.0f, 20000.0f));

      // Calculate sample
      float currentSample = osc.getNextSample();

      // Distortion stage
      float currentCleanSample = currentSample;
      currentSample =
        juce::dsp::FastMathApproximations::tanh(5.0f * currentSample);
      currentSample = (0.8 * currentSample) + (0.2 * currentCleanSample);

      // Calculate gain
      auto oscGain = gainEnvelope.getNextSample();
      currentSample = currentSample * oscGain;

      // Write final sample
      leftChannel[sample] = currentSample;
      rightChannel[sample] = currentSample;
    }
  }
  //============================================================================
  void setParams(SynthParameters param) { this->synthParams = param; };
  //============================================================================
private:
  SynthParameters synthParams;
  dmt::AnalogOscillator osc;
  dmt::AhdEnvelope gainEnvelope;
  dmt::AhdEnvelope pitchEnvelope;
  float baseFreq = 0.0f;
  float pitchDepth = 0.7;
  bool isPrepared = false;

  void setEnvelopes()
  {
    dmt::AhdEnvelope::Parameters params;
    params.attack = 0.0f;
    params.hold = 0.08f;
    params.decay = 0.5f;
    params.attackScew = 0;
    params.decayScew = 0;
    gainEnvelope.setParameters(params);
    params.attack = 0.0f;
    params.hold = 0.0f;
    params.decay = synthParams.modDecay;
    params.decayScew = synthParams.modScew;
    pitchEnvelope.setParameters(params);
  }
};

}