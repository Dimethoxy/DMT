/*
  ==============================================================================

    FilterProcessor.h
    Created: 3 Mar 2023 4:16:29pm
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
//==============================================================================
class FilterProcessor
{
public:
  void onNote()
  {
    // filter.
  }
  void prepare(double sampleRate)
  {
    auto cof = juce::IIRCoefficients::makeLowPass(sampleRate, 1000.0f, 0.1f);
    filterBase.setCoefficients(cof);
  }
  void processBlock(juce::AudioBuffer<float>& outputBuffer,
                    int startSample,
                    int numSamples)
  {
    auto endSample = numSamples + startSample;
    auto* leftChannel = outputBuffer.getWritePointer(0);
    auto* rightChannel = outputBuffer.getWritePointer(1);
    for (int sample = startSample; sample < endSample; sample++) {
      leftChannel[sample] =
        filterBase.processSingleSampleRaw(leftChannel[sample]);
      rightChannel[sample] = leftChannel[sample];
    }
  }

private:
  juce::SingleThreadedIIRFilter::IIRFilterBase filterBase;
};
}