//==============================================================================

#pragma once

#include "../Filter/IIRFilter.h"
#include "../Synth/AnalogWaveform.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
struct ChainSettings
{
  //============================================================================
  ChainSettings(juce::AudioProcessorValueTreeState& apvts)
  {
    waveformType = static_cast<dmt::AnalogWaveform::Type>(
      apvts.getRawParameterValue("waveformType")->load());

    oscGain = apvts.getRawParameterValue("oscGain")->load();
    oscDrive = apvts.getRawParameterValue("oscDrive")->load();
    oscBias = apvts.getRawParameterValue("oscBias")->load();

    ampAttack = apvts.getRawParameterValue("oscAmpAttack")->load();
    ampHold = apvts.getRawParameterValue("oscAmpHold")->load();
    ampDecay = apvts.getRawParameterValue("oscAmpDecay")->load();

    modDecay = apvts.getRawParameterValue("oscModDecay")->load();
    modDepth = apvts.getRawParameterValue("oscModDepth")->load();
    modScew = apvts.getRawParameterValue("oscModScew")->load();

    filterType = static_cast<dmt::IIRFilterState::Type>(
      apvts.getRawParameterValue("filterType")->load());
    filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
    filterQ = apvts.getRawParameterValue("filterQ")->load();
    filterGain = apvts.getRawParameterValue("filterGain")->load();
  }
  //============================================================================
  dmt::AnalogWaveform::Type waveformType;

  float oscGain;
  float oscDrive;
  float oscBias;
  float oscPwm;

  float ampAttack;
  float ampHold;
  float ampDecay;

  float modDecay;
  float modDepth;
  float modScew;

  dmt::IIRFilterState::Type filterType;
  float filterCutoff;
  float filterQ;
  float filterGain;
  //============================================================================
};
}