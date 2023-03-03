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
  void onNote() { jassert("Hi"); }

private:
  juce::dsp::BallisticsFilter<float> filter;
};
}