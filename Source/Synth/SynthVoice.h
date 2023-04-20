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
    note = midiNoteNumber;
    ;

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

    osc.setWaveformType(chainSettings->waveformType);
    osc.setDrive(chainSettings->oscDrive);
    osc.setBias(chainSettings->oscBias);
    osc.setBend(chainSettings->oscBend);
    osc.setPwm(chainSettings->oscPwm);
    osc.setSync(chainSettings->oscSync);

    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = startSample; sample < endSample; sample++) {
      osc.setFrequency(getFrequency());
      float currentSample = osc.getNextSample();
      gain(currentSample);
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
  //============================================================================
private:
  std::unique_ptr<dmt::ChainSettings> chainSettings;
  dmt::AnalogOscillator osc;
  dmt::AhdEnvelope gainEnvelope;
  dmt::AhdEnvelope pitchEnvelope;
  int note = 0;
  float pitchDepth = 0.7;
  bool isPrepared = false;

  std::vector<std::function<void()>> onNoteReceivers;
  //============================================================================
  float getFrequency()
  {
    int semitones = (8 * chainSettings->oscOctave) + chainSettings->oscSemitone;
    float baseFreq = juce::MidiMessage::getMidiNoteInHertz(note + semitones);
    float freqModDepth =
      chainSettings->oscOctave * baseFreq + chainSettings->modDepth;
    float envelopeSample = pitchEnvelope.getNextSample();
    float newFreq = juce::mapToLog10(envelopeSample, baseFreq, freqModDepth);
    return std::clamp(newFreq, 20.0f, 20000.0f);
  }

  void gain(float& currentSample)
  {
    float envGain = gainEnvelope.getNextSample();
    float oscGain =
      juce::Decibels::decibelsToGain(chainSettings->oscGain, -96.0f);
    currentSample = currentSample * envGain * oscGain;
  }
  //============================================================================
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
  //============================================================================
};

}
//==============================================================================
