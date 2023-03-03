//==============================================================================

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
struct ChainSettings
{
  //============================================================================
  ChainSettings(juce::AudioProcessorValueTreeState& apvts)
  {
    oscGain = apvts.getRawParameterValue("oscGain")->load();
    oscDrive = apvts.getRawParameterValue("oscDrive")->load();

    ampAttack = apvts.getRawParameterValue("oscAmpAttack")->load();
    ampHold = apvts.getRawParameterValue("oscAmpHold")->load();
    ampDecay = apvts.getRawParameterValue("oscAmpDecay")->load();

    modDecay = apvts.getRawParameterValue("oscModDecay")->load();
    modDepth = apvts.getRawParameterValue("oscModDepth")->load();
    modScew = apvts.getRawParameterValue("oscModScew")->load();
  }
  //============================================================================
  float oscGain;
  float oscDrive;

  float ampAttack;
  float ampHold;
  float ampDecay;

  float modDecay;
  float modDepth;
  float modScew;
  //============================================================================
};
}