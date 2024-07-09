//==============================================================================

#pragma once

#include "envelope/AdhEnvelope.h"
#include "model/ChainSettings.h"
#include "synth/AnalogOscillator.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace dsp {
namespace synth {
//==============================================================================
class SynthVoice : public juce::SynthesiserVoice
{

public:
  SynthVoice(juce::AudioProcessorValueTreeState& apvts)
    : apvts(apvts)
  {
  }
  //============================================================================
  bool canPlaySound(juce::SynthesiserSound* sound) override
  {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
  }
  void controllerMoved(int /*controllerNumber*/,
                       int /*newControllerValue*/) override
  {
  }
  void pitchWheelMoved(int /*newPitchWheelValue*/) override {}
  //============================================================================
  void prepareToPlay(double sampleRate,
                     int /*samplesPerBlock*/,
                     int /*outputChannels*/)
  {
    // Exit if the sample rate is invalid
    if (sampleRate <= 0)
      return;

    // Prepare envelope
    gainEnvelope.setSampleRate((float)sampleRate);
    pitchEnvelope.setSampleRate((float)sampleRate);

    // Prepare oscillator
    osc.setSampleRate((float)sampleRate);

    // Set state as prepared
    isPrepared = true;
  }
  //============================================================================
  void startNote(int midiNoteNumber,
                 float /*velocity*/,
                 juce::SynthesiserSound* /*sound*/,
                 int /*currentPitchWheelPosition*/) override
  {
    // Set oscillator frequency
    osc.setPhase(0.0f);
    note = midiNoteNumber;

    // Start envelopes
    updateEnvelopeParameters();
    gainEnvelope.noteOn();
    pitchEnvelope.noteOn();

    callOnNoteReceivers();
  }
  void stopNote(float /*velocity*/, bool /*allowTailOff*/) override {}
  //============================================================================
  void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                       int startSample,
                       int numSamples) override
  {
    // Exit if the voice is not playing or the voice is not prepared
    if (!isVoiceActive() || !isPrepared)
      return;

    // Update envelope parameters
    updateEnvelopeParameters();

    // Set oscillator parameters
    updateOscillatorParameters();

    // Buffer parameters
    const float oscGain =
      apvts.getRawParameterValue("osc1DistortionPreGain")->load();
    const int oscOctave = apvts.getRawParameterValue("osc1VoiceOctave")->load();
    const int oscSemitone =
      apvts.getRawParameterValue("osc1VoiceSemitone")->load();
    const float oscModDepth =
      apvts.getRawParameterValue("osc1PitchEnvDepth")->load();

    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = startSample; sample < endSample; sample++) {
      osc.setFrequency(getNextFrequency(oscOctave, oscSemitone, oscModDepth));
      const auto rawSample = osc.getNextSample();
      const auto gainedSample = applyGain(rawSample, oscGain);
      leftChannel[sample] = gainedSample;
      rightChannel[sample] = gainedSample;
    }
  }
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

protected:
  //============================================================================
  void updateEnvelopeParameters()
  {
    // Gain envelope
    dmt::dsp::envelope::AhdEnvelope::Parameters gainEnvParameters;
    gainEnvParameters.attack =
      apvts.getRawParameterValue("osc1GainEnvAttack")->load();
    gainEnvParameters.hold =
      apvts.getRawParameterValue("osc1GainEnvHold")->load();
    gainEnvParameters.decay =
      apvts.getRawParameterValue("osc1GainEnvDecay")->load();
    gainEnvParameters.decayScew =
      apvts.getRawParameterValue("osc1GainEnvSkew")->load();
    gainEnvParameters.attackScew = 0;
    gainEnvelope.setParameters(gainEnvParameters);

    // Pitch envelope
    dmt::dsp::envelope::AhdEnvelope::Parameters pitchEnvParameters;
    pitchEnvParameters.attack = 0;
    pitchEnvParameters.hold =
      apvts.getRawParameterValue("osc1PitchEnvHold")->load();
    pitchEnvParameters.decay =
      apvts.getRawParameterValue("osc1PitchEnvDecay")->load();
    pitchEnvParameters.decayScew =
      apvts.getRawParameterValue("osc1PitchEnvSkew")->load();
    pitchEnvParameters.attackScew = 0;
    pitchEnvelope.setParameters(pitchEnvParameters);
  }
  //============================================================================
  void updateOscillatorParameters()
  {
    osc.setWaveformType(static_cast<dmt::dsp::synth::AnalogWaveform::Type>(
      apvts.getRawParameterValue("osc1WaveformType")->load()));
    osc.setDrive(apvts.getRawParameterValue("osc1DistortionType")->load());
    osc.setBias(apvts.getRawParameterValue("osc1DistortionSymmetry")->load());
    osc.setBend(apvts.getRawParameterValue("osc1WaveformBend")->load());
    osc.setPwm(apvts.getRawParameterValue("osc1WaveformPwm")->load());
    osc.setSync(apvts.getRawParameterValue("osc1WaveformSync")->load());
  }
  //============================================================================
  float getNextFrequency(const int rawOctave,
                         const int rawSemitone,
                         const float rawModDepth)
  {
    const int octaves = 12 * rawOctave;
    const int semitone = octaves + rawSemitone;
    const int baseNote = note + semitone;
    const float baseFreq = juce::MidiMessage::getMidiNoteInHertz(baseNote);
    const float modDepth = rawModDepth * 2e4f;
    const float envelopeSample = pitchEnvelope.getNextSample();
    const float maxFreq = std::clamp(baseFreq + modDepth, baseFreq, 2e4f);
    const float newFreq = juce::mapToLog10(envelopeSample, baseFreq, maxFreq);
    return std::clamp(newFreq, 20.0f, 2e4f);
  }
  //============================================================================
  const float applyGain(float sample, float oscGain)
  {
    const float envGain = gainEnvelope.getNextSample();
    const float gain = juce::Decibels::decibelsToGain(oscGain, -96.0f);
    return sample * envGain * gain;
  }
  //============================================================================
private:
  juce::AudioProcessorValueTreeState& apvts;
  dmt::dsp::synth::AnalogOscillator osc;
  dmt::dsp::envelope::AhdEnvelope gainEnvelope;
  dmt::dsp::envelope::AhdEnvelope pitchEnvelope;
  int note = 0;
  bool isPrepared = false;
  std::vector<std::function<void()>> onNoteReceivers;
};

} // namespace synth
} // namespace dsp
} // namespace dmt
//==============================================================================
