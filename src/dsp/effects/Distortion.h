#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace distortion {
struct Distortion
{
  enum class Type
  {
    Hardclip,
    Softclip,
    Saturate,
    Atan,
    Crunch,
    Extreme,
    Scream,
    Sine,
    Cosine,
    Harmonize,
    Weird,
    Bitcrush,
  };

  static inline const juce::String getString(const Type type)
  {
    switch (type) {
      case Distortion::Type::Hardclip:
        return ("Hardclip");
        break;
      case Distortion::Type::Softclip:
        return ("Softclip");
        break;
      case Distortion::Type::Saturate:
        return ("Saturate");
        break;
      case Distortion::Type::Atan:
        return ("Atan");
        break;
      case Distortion::Type::Crunch:
        return ("Crunch");
        break;
      case Distortion::Type::Extreme:
        return ("Extreme");
        break;
      case Distortion::Type::Scream:
        return ("Scream");
        break;
      case Distortion::Type::Sine:
        return ("Sine");
        break;
      case Distortion::Type::Cosine:
        return ("Cosine");
        break;
      case Distortion::Type::Harmonize:
        return ("Harmonize");
        break;
      case Distortion::Type::Weird:
        return ("Weird");
        break;
      case Distortion::Type::Bitcrush:
        return ("Bitcrush");
        break;
      default:
        // impossible to reach exit with assertion
        jassert(false);
        return ("Hardclip");
        break;
    }
  }

  template<typename ValueType,
           typename = std::enable_if_t<std::is_floating_point_v<ValueType>>>
  static inline const ValueType distortSample(const ValueType value,
                                              const Type type,
                                              const ValueType drive)
  {
    switch (type) {
      case Distortion::Hardclip: {
        return std::clamp(drive * value, -1.0, 1.0);
      }
      case Distortion::Softclip: {
        float threshold1 = 1.0f / 3.0f;
        float threshold2 = 2.0f / 3.0f;
        const auto newValue = drive * value;
        if (newValue > threshold2)
          return 1.0f;
        else if (newValue > threshold1)
          return 1.0f - powf(2.0f - 3.0f * newValue, 2.0f) / 3.0f;
        else if (newValue < -threshold2)
          return = -1.0f;
        else if (newValue < -threshold1)
          return -1.0f + powf(2.0f + 3.0f * newValue, 2.0f) / 3.0f;
        else
          return 2.0f * newValue;
      }
    }

    // Make sure to return a value at the end of the function
    // (replace the return statement with the appropriate value)
    return /* some value */;
  }

  static inline float getNewGirthSeed() { return (float)(rand() % 100); }

  static inline std::vector<float> getGirthSeeds(int numSamples)
  {
    std::vector<float> girthSeeds;
    for (int i = 0; i < numSamples; i++) {
      girthSeeds.push_back(getNewGirthSeed());
    }
    return girthSeeds;
  }

  static inline void girthSample(float& value, float girth)
  {
    value = value * ((getNewGirthSeed() / 100 * girth) + 1);
    value = std::clamp(value, -1.0f, 1.0f);
  }

  static inline void girthSample(float& value, float girth, float seed)
  {
    value = value * (((seed) / 100 * girth) + 1);
    value = std::clamp(value, -1.0f, 1.0f);
  }

  static inline void symmetrySample(float& value, float symmetry)
  {
    if (value > 0) {
      value += value * symmetry;
    } else {
      value -= value * symmetry;
    }
    value = std::clamp(value, -1.0f, 1.0f);
  }

  static inline void processBuffer(juce::AudioBuffer<float>& buffer,
                                   Type type,
                                   float symmetry,
                                   float girth,
                                   float drive)
  {
    std::vector<float> girthSeeds;
    if (girth < 0)
      girthSeeds = getGirthSeeds(buffer.getNumSamples());

    for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
      auto* channelData = buffer.getWritePointer(channel);
      for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        // Girth
        if (girth < 0)
          girthSample(channelData[sample], std::abs(girth), girthSeeds[sample]);
        else
          girthSample(channelData[sample], girth);

        // Distortion
        distortSample(channelData[sample], type, drive);

        // Symmetry
        symmetrySample(channelData[sample], symmetry);
      }
    }
  }
};
} // namespace distortion
} // namespace dsp
} // namespace dmt
