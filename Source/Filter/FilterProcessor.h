//==============================================================================

#pragma once

#include "../Utility/ChainSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
struct IIRFilter
{
public:
  enum Type
  {
    LowPass,
    HighPass,
    BandPass,
    Notch,
    AllPass,
    LowShelf,
    HighShelf,
    Peak,
  };
  juce::IIRCoefficients makeCoefficients(float cutoff, float q, float gain)
  {
    switch (type) {
      case LowPass:
        return juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, q);
        break;
      case HighPass:
        break;
      case BandPass:
        break;
      case Notch:
        break;
      case AllPass:
        break;
      case LowShelf:
        break;
      case HighShelf:
        break;
      case Peak:
        break;
    }
  }
  float sampleRate = -1;
  Type type = Type::LowPass;
  juce::SingleThreadedIIRFilter::IIRFilterBase base;
};
//==============================================================================
class FilterProcessor
{
public:
  void onNote()
  {
    // Reset Envelope
  }
  void prepare(double sampleRate)
  {
    this->sampleRate = sampleRate;
    this->filter.sampleRate = sampleRate;
  }
  void processBlock(juce::AudioBuffer<float>& outputBuffer,
                    int startSample,
                    int numSamples)
  {
    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);

    float filterBaseCutoff = chainSettings->filterCutoff;
    auto coefficients = filter.makeCoefficients(filterBaseCutoff, 1.0f, 1.0f);
    filter.base.setCoefficients(coefficients);

    for (int sample = startSample; sample < endSample; sample++) {
      leftChannel[sample] =
        filter.base.processSingleSampleRaw(leftChannel[sample]);
      rightChannel[sample] = leftChannel[sample];
    }
  }
  //============================================================================
  void setChainSettings(dmt::ChainSettings chainSettings)
  {
    this->chainSettings = std::make_unique<dmt::ChainSettings>(chainSettings);
  };

private:
  float sampleRate = -1;
  std::unique_ptr<dmt::ChainSettings> chainSettings;
  dmt::IIRFilter filter;
};
}