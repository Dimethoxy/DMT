#pragma once

#include <JuceHeader.h>

namespace dmt {
struct Distortion
{
  static inline enum class Type {
    Hardclip,
    Root,
    Atan,
    Crunch,
    Bitcrush,
    Sine,
    Cosine,
    Expand,
    Weird,
    Extreme
  };

  static inline juce::String getString(dmt::Distortion::Type type)
  {
    switch (type) {
      case Type::Hardclip:
        return ("Hardclip");
        break;
      case Type::Root:
        return ("Sqrt");
        break;
      case Type::Atan:
        return ("Atan");
        break;
      case Type::Crunch:
        return ("Crunch");
        break;
      case Type::Bitcrush:
        return ("Bitcrush");
        break;
      case Type::Sine:
        return ("Sine");
        break;
      case Type::Cosine:
        return ("Cosine");
        break;
      case Type::Expand:
        return ("Expand");
        break;
      case Type::Weird:
        return ("Weird");
        break;
      case Type::Extreme:
        return ("Extreme");
        break;
    }
  }

  static inline void distortSample(float& value,
                                   dmt::Distortion::Type type,
                                   float drive)
  {
    switch (type) {
      case Type::Hardclip: {
        value = value * drive;
        value = std::clamp(value, -1.0f, 1.0f);
        break;
      }
      case Type::Root: {
        if (value > 0.0) {
          value = std::pow(value, 1.0 / ((drive / 4) + 0.75));
          value = std::clamp(value, -1.0f, 1.0f);
        } else {
          value = pow(-value, 1.0 / ((drive / 4) + 0.75));
          value = -std::clamp(value, -1.0f, 1.0f);
        }
        break;
      }
      case Type::Atan: {
        if (value != 0.0f) {
          if (value > 0.0f) {
            value = pow(value, 1.0f / drive);
            value = 1.27f * atan(value);
            value = std::clamp(value, -1.0f, 1.0f);
          } else {
            value = pow(-value, 1.0f / drive);
            value = 1.27f * atan(value);
            value = -value;
            value = std::clamp(value, -1.0f, 1.0f);
          }
        }
        break;
      }
      case Type::Crunch: {
        if (value > 0.0) {
          value = pow(value, 1.0f / drive);
          value = 1.27 * atan(value);
          value = std::clamp(value, -1.0f, 1.0f);
        } else {
          value = std::clamp(sin(drive * value), -1.0f, 1.0f);
          value = std::clamp(drive * value, -1.0f, 1.0f);
        };
        break;
      }
      case Type::Bitcrush: {
        float bitDepth = 11.0 - drive;
        float exponent = bitDepth - 1;
        float possibleValues = pow(2, exponent);
        float quantized = value * possibleValues;
        if (quantized > 0) {
          quantized = ceil(quantized);
        } else if (quantized < 0) {
          quantized = floor(quantized);
        }
        value = quantized / possibleValues;
        break;
      }
      case Type::Sine: {
        value = std::clamp(sin(drive * value), -1.0f, 1.0f);
        break;
      }
      case Type::Cosine: {
        value = std::clamp(cos(drive * value), -1.0f, 1.0f);
        break;
      }
      case Type::Expand: {
        if (std::abs(value) >= ((drive - 1) / 9.0f)) {
          auto signbit = (std::signbit(value) ? -1 : 1);
          value = signbit;
        }
        break;
      }
      case Type::Weird: {
        float invertedDrive = (10.0f / drive) * 5.0f;
        value = value * invertedDrive;
        float h1 = sin(2.0f * value);
        float h2 = sin(3.0f * value);
        float h3 = sin(4.0f * value);
        value = (h1 + h2 + h3 + value) / invertedDrive;
        value = std::clamp(value, -1.0f, 1.0f);
        break;
      }
      case Type::Extreme: {
        value = value * (drive * 2);
        float h1 = sin(2 * value);
        float h2 = sin(3 * value);
        float h3 = sin(4 * value);
        value = sin(h1 + h2 + h3 + value);
        value = std::clamp(value, -1.0f, 1.0f);
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
    } else if (value) {
      value -= value * symmetry;
    }
    value = std::clamp(value, -1.0f, 1.0f);
  }

  static inline void processBuffer(juce::AudioBuffer<float>& buffer,
                                   dmt::Distortion::Type type,
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
          dmt::Distortion::girthSample(
            channelData[sample], std::abs(girth), girthSeeds[sample]);
        else
          dmt::Distortion::girthSample(channelData[sample], girth);

        // Distortion
        dmt::Distortion::distortSample(channelData[sample], type, drive);

        // Symmetry
        dmt::Distortion::symmetrySample(channelData[sample], symmetry);
      }
    }
  }
};
}