//==============================================================================

#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace filter {
struct IIRFilterState
{
  using IIRCoefficients = juce::IIRCoefficients;
  enum class Type
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
  static const inline juce::StringArray typeNames = { "LowPass",   "HighPass",
                                                      "BandPass",  "Notch",
                                                      "AllPass",   "LowShelf",
                                                      "HighShelf", "Peak" };

  juce::IIRCoefficients makeCoefficients(float cutoff, float q, float gain)
  {
    switch (type) {
      case Type::LowPass:
        return IIRCoefficients::makeLowPass(sampleRate, cutoff, q);
        break;
      case Type::HighPass:
        return IIRCoefficients::makeHighPass(sampleRate, cutoff, q);
        break;
      case Type::BandPass:
        return IIRCoefficients::makeBandPass(sampleRate, cutoff, q);
        break;
      case Type::Notch:
        return IIRCoefficients::makeNotchFilter(sampleRate, cutoff, q);
        break;
      case Type::AllPass:
        return IIRCoefficients::makeAllPass(sampleRate, cutoff, q);
        break;
      case Type::LowShelf:
        return IIRCoefficients::makeLowShelf(sampleRate, cutoff, q, gain);
        break;
      case Type::HighShelf:
        return IIRCoefficients::makeHighShelf(sampleRate, cutoff, q, gain);
        break;
      case Type::Peak:
        return IIRCoefficients::makePeakFilter(sampleRate, cutoff, q, gain);
        break;
      default:
        // impossible to reach exit with assertion
        jassert(false);
        return IIRCoefficients();
    }
  }
  float sampleRate = -1;
  Type type = Type::LowPass;
  juce::SingleThreadedIIRFilter::IIRFilterBase base;
};
} // namespace filter
} // namespace dsp
} // namespace dmt