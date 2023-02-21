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
    osc.reset();
    note = midiNoteNumber;
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
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    osc.prepare(spec);
    gain.prepare(spec);
    osc.setFrequency(60.0f);
    gain.setGainLinear(1.0f);

    gainEnvelope.setSampleRate(sampleRate);
    pitchEnvelope.setSampleRate(sampleRate);

    isPrepared = true;
  }

  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
    // Exit if the voice is not playing
    if (!isVoiceActive() || !isPrepared)
      return;

    // Reallocate and clear memory
    tempBuffer.setSize(
      outputBuffer.getNumChannels(), numSamples, false, false, true);
    tempBuffer.clear();
    tempBuffer.clear();

    // Render Oscillator
    auto* leftChannel = tempBuffer.getWritePointer(0);
    auto* rightChannel = tempBuffer.getWritePointer(1);
    for (int sample = 0; sample < tempBuffer.getNumSamples(); sample++) {
      // Calculate frquency
      osc.setFrequency(note);
      float baseFreq = osc.getFrequency();
      float maxFreqModDepth = synthParams.modDepth;
      float freqModDepth = maxFreqModDepth * pitchEnvelope.getNextSample();
      osc.setFrequency(baseFreq + freqModDepth);

      // Calculate gain
      auto oscGain = gainEnvelope.getNextSample();
      auto currentSample = osc.processSample(1.0f) * oscGain;

      // Write final sample
      leftChannel[sample] = currentSample;
      rightChannel[sample] = currentSample;
    }

    // Add temporary buffer to output buffer
    for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
      outputBuffer.addFrom(
        channel, startSample, tempBuffer, channel, 0, numSamples);
    }
  }

private:
  float wave(float x)
  {
    const auto pi = juce::MathConstants<float>::pi;
    auto signBit = std::signbit(x) ? -1 : 1;
    auto result = x * signBit / pi - pi;
    return sin(x);
  }
  SynthParameters synthParams;
  juce::dsp::Oscillator<float> osc{ [&](float x) { return wave(x); } };
  juce::dsp::Gain<float> gain;
  dmt::AhdEnvelope gainEnvelope;
  dmt::AhdEnvelope pitchEnvelope;
  int note = 0;
  float pitchDepth = 0.7;
  juce::AudioBuffer<float> tempBuffer;
  bool isPrepared = false;
};

}