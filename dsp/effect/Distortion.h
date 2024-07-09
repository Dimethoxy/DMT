#pragma once

#include <JuceHeader.h>

namespace dmt {
namespace dsp {
namespace effect {
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

  static inline void distortSample(float& data, const Type type, float drive)
  {
    switch (type) {
      case Type::Hardclip: {
        data = std::clamp(drive * data, -1.0f, 1.0f);
        break;
      }
      case Type::Softclip: {
        float threshold1 = 1.0f / 3.0f;
        float threshold2 = 2.0f / 3.0f;

        data *= drive;
        if (data > threshold2)
          data = 1.0f;
        else if (data > threshold1)
          data = 1.0f - powf(2.0f - 3.0f * data, 2.0f) / 3.0f;
        else if (data < -threshold2)
          data = -1.0f;
        else if (data < -threshold1)
          data = -1.0f + powf(2.0f + 3.0f * data, 2.0f) / 3.0f;
        else
          data = 2.0f * data;

        break;
      }
      case Type::Saturate: {
        if (data > 0.0) {
          data = std::clamp(pow(data, 1.0 / ((drive / 4) + 0.75)), -1.0, 1.0);
        } else {
          data = -std::clamp(pow(-data, 1.0 / ((drive / 4) + 0.75)), -1.0, 1.0);
        }
        break;
      }
      case Type::Atan: {
        if (std::abs(data) <= 1.0e-05) {
          if (data > 0.0f) {
            data = pow(data, 1.0f / drive);
            data = 1.27f * atan(data);
          } else {
            data = pow(-data, 1.0f / drive);
            data = 1.27f * atan(data);
            data = -data;
          }
        }
        break;
      }
      case Type::Crunch: {
        if (data > 0.0) {
          data = pow(data, 1.0f / drive);
          data = 1.27 * atan(data);
        } else {
          data = std::clamp((float)sin(drive * data), -1.0f, 1.0f);
          data = std::clamp(drive * data, -1.0f, 1.0f);
        };
        break;
      }
      case Type::Extreme: {
        float invertedDrive = 10.0f - (drive - 1);
        if (std::abs(data) >= ((invertedDrive - 1) / 9.0f)) {
          auto signbit = (std::signbit(data) ? -1 : 1);
          data = signbit;
        }
        break;
      }
      case Type::Scream: {
        auto temp = data;
        auto normalizedDrive = (drive - 1) / 10.0f;

        if (data > 0.0) {
          data = std::clamp(
            (float)pow(data, 1.0f / ((drive / 4.0f) + 0.75f)), -1.0f, 1.0f);
        } else {
          data = -std::clamp(
            (float)pow(-data, 1.0f / ((drive / 4.0f) + 0.75f)), -1.0f, 1.0f);
        }

        if (data <= -0.5) {
          auto offset = 3.0;
          temp = 4 * data + offset;
        } else if (data > -0.5 && data < 0.5) {
          temp = -2 * data;
        } else {
          auto offset = -3.0;
          temp = 4 * data + offset;
        }

        data = (temp * normalizedDrive) + (data * (1 - normalizedDrive));

        break;
      }
      case Type::Sine: {
        data = std::clamp((float)sin(drive * data), -1.0f, 1.0f);
        break;
      }
      case Type::Cosine: {
        data = std::clamp((float)cos(drive * data), -1.0f, 1.0f);
        break;
      }
      case Type::Harmonize: {
        data = data * (drive * 5);
        float h1 = sin(2 * data);
        float h2 = sin(3 * data);
        float h3 = sin(4 * data);
        data = (h1 + h2 + h3 + data) / (drive * 5);
        break;
      }
      case Type::Weird: {
        data = data * (drive * 2);
        float h1 = sin(2 * data);
        float h2 = sin(3 * data);
        float h3 = sin(4 * data);
        data = sin(h1 + h2 + h3 + data);
        break;
      }
      case Type::Bitcrush: {
        float bitDepth = 10.0f - (drive - 1);
        float exponent = bitDepth - 1;
        float possibleValues = pow(2, exponent);
        float quantized = (data + 1.0f) * possibleValues;
        quantized = round(quantized);
        data = (quantized / possibleValues) - 1.0f;
        break;
      }
    }
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
} // namespace effect
} // namespace dsp
} // namespace dmt