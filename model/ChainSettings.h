//==============================================================================

#pragma once

#include "../dsp/filter/IIRFilter.h"
#include "../dsp/synth/AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
struct ChainSettings
{
  //============================================================================
  ChainSettings(juce::AudioProcessorValueTreeState& /*apvts*/)
  {
    // waveformType = static_cast<dmt::dsp::synth::AnalogWaveform::Type>(
    //   apvts.getRawParameterValue("waveformType")->load());
    //
    // oscGain = apvts.getRawParameterValue("oscGain")->load();
    // oscOctave = apvts.getRawParameterValue("oscOctave")->load();
    // oscSemitone = apvts.getRawParameterValue("oscSemitone")->load();
    // oscDrive = apvts.getRawParameterValue("oscDrive")->load();
    // oscBias = apvts.getRawParameterValue("oscBias")->load();
    // oscBend = apvts.getRawParameterValue("oscBend")->load();
    // oscPwm = apvts.getRawParameterValue("oscPwm")->load();
    // oscSync = apvts.getRawParameterValue("oscSync")->load();
    //
    // ampAttack = apvts.getRawParameterValue("oscAmpAttack")->load();
    // ampHold = apvts.getRawParameterValue("oscAmpHold")->load();
    // ampDecay = apvts.getRawParameterValue("oscAmpDecay")->load();
    //
    // modDecay = apvts.getRawParameterValue("oscModDecay")->load();
    // modDepth = apvts.getRawParameterValue("oscModDepth")->load();
    // modScew = apvts.getRawParameterValue("oscModScew")->load();
    //
    // filterType = static_cast<dmt::dsp::filter::IIRFilterState::Type>(
    //   apvts.getRawParameterValue("filterType")->load());
    // filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
    // filterQ = apvts.getRawParameterValue("filterQ")->load();
    // filterGain = apvts.getRawParameterValue("filterGain")->load();
  }
  //============================================================================
  dmt::dsp::synth::AnalogWaveform::Type waveformType;

  float oscGain;
  float oscOctave;
  float oscSemitone;
  float oscDrive;
  float oscBias;
  float oscBend;
  float oscPwm;
  float oscSync;

  float ampAttack;
  float ampHold;
  float ampDecay;

  float modDecay;
  float modDepth;
  float modScew;

  dmt::dsp::filter::IIRFilterState::Type filterType;
  float filterCutoff;
  float filterQ;
  float filterGain;
  //============================================================================
};
} // namespace dmt
