//==============================================================================

#pragma once

#include "../../model/ChainSettings.h"
#include "IIRFilter.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace dsp {
namespace filter {
//==============================================================================
class FilterProcessor
{
public:
  void onNote()
  {
    // Reset Envelope
  }
  void prepare(double newSampleRate)
  {
    this->sampleRate = (float)newSampleRate;
    this->filter.sampleRate = (float)newSampleRate;
  }
  void processBlock(juce::AudioBuffer<float>& outputBuffer,
                    int startSample,
                    int numSamples)
  {
    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);

    filter.type = chainSettings->filterType;
    float filterBaseCutoff = chainSettings->filterCutoff;
    auto coefficients = filter.makeCoefficients(
      filterBaseCutoff, chainSettings->filterQ, chainSettings->filterGain);
    filter.base.setCoefficients(coefficients);

    for (int sample = startSample; sample < endSample; sample++) {
      // leftChannel[sample] =
      //   filter.base.processSingleSampleRaw(leftChannel[sample]);
      rightChannel[sample] = leftChannel[sample];
    }
  }
  //============================================================================
  void setChainSettings(dmt::ChainSettings newChainSettings)
  {
    this->chainSettings =
      std::make_unique<dmt::ChainSettings>(newChainSettings);
  };

private:
  float sampleRate = -1;
  std::unique_ptr<dmt::ChainSettings> chainSettings;
  dmt::dsp::filter::IIRFilterState filter;
};
} // namespace filter
} // namespace dsp
} // namespace dmt