//==============================================================================

#pragma once

#include "../Envelope/AdhEnvelope.h"
#include "../Synth/AnalogOscillator.h"
#include "../Utility/ChainSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class SynthVoice : public juce::SynthesiserVoice
{
  using Math = juce::dsp::FastMathApproximations;

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
    baseFreq = 12 + midiNoteNumber;

    // Start envelopes
    setEnvelopes();
    gainEnvelope.noteOn();
    pitchEnvelope.noteOn();

    callOnNoteReceivers();
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
    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    osc.setWaveformType(chainSettings->waveformType);
    for (int sample = startSample; sample < endSample; sample++) {
      // Frequency
      float freqModDepth = baseFreq + chainSettings->modDepth;
      float envelopeSample = pitchEnvelope.getNextSample();
      float newFreq = juce::mapToLog10(envelopeSample, baseFreq, freqModDepth);
      osc.setFrequency(std::clamp(newFreq, 20.0f, 20000.0f));
     
      // Calculate sample
      float currentSample = osc.getNextSample();

      // Bias
      float bias = chainSettings->oscBias;
      float sign = (currentSample > 0.0f) ? 1.0f : -1.0f;
      currentSample = Math::tanh(chainSettings->oscDrive * currentSample);

      // Distortion
      currentSample = currentSample + sign * currentSample * bias;

      // Gain
      float envGain = gainEnvelope.getNextSample();
      float oscGain =
        juce::Decibels::decibelsToGain(chainSettings->oscGain, -96.0f);
      currentSample = currentSample * envGain * oscGain;

      // Write final sample
      leftChannel[sample] = currentSample;
      rightChannel[sample] = currentSample;
    }
  }
  //============================================================================
  void setChainSettings(dmt::ChainSettings chainSettings)
  {
    this->chainSettings = std::make_unique<dmt::ChainSettings>(chainSettings);
  };
  //============================================================================
  void addOnNoteReceivers(std::function<void()> callbackFunction)
  {
    onNoteReceivers.push_back(callbackFunction);
  }
  void callOnNoteReceivers()
  {
    for (std::function<void()> func : onNoteReceivers) {
      func();
    }
  }

private:
  std::unique_ptr<dmt::ChainSettings> chainSettings;
  dmt::AnalogOscillator osc;
  dmt::AhdEnvelope gainEnvelope;
  dmt::AhdEnvelope pitchEnvelope;
  float baseFreq = 0.0f;
  float pitchDepth = 0.7;
  bool isPrepared = false;

  std::vector<std::function<void()>> onNoteReceivers;

  void setEnvelopes()
  {
    dmt::AhdEnvelope::Parameters envParameters;
    envParameters.attack = chainSettings->ampAttack;
    envParameters.hold = chainSettings->ampHold;
    envParameters.decay = chainSettings->ampDecay;
    envParameters.attackScew = 0;
    envParameters.decayScew = 0;
    gainEnvelope.setParameters(envParameters);
    envParameters.attack = 0.0;
    envParameters.hold = 0.0f;
    envParameters.decay = chainSettings->modDecay;
    envParameters.decayScew = chainSettings->modScew;
    pitchEnvelope.setParameters(envParameters);
  }
};

}
//==============================================================================