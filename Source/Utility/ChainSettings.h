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

    filterType = static_cast<dmt::IIRFilter::Type>(
      apvts.getRawParameterValue("filterCutoff")->load());
    filterCutoff = apvts.getRawParameterValue("filterCutoff")->load();
  }
  //============================================================================
  dmt::AnalogWaveform::Type waveformType;

  float oscGain;
  float oscDrive;
  float oscBias;

  float ampAttack;
  float ampHold;
  float ampDecay;

  float modDecay;
  float modDepth;
  float modScew;

  dmt::IIRFilter::Type filterType;
  float filterCutoff;
  //============================================================================
};
}