//==============================================================================

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace Math {
//==============================================================================

static inline float
linearToExponent(float value) noexcept
{
  return (value >= 0.0f) ? (value + 1.0f) : (1.0f / -(value + 1.0f));
}

static inline juce::Point<float>
pointOnCircle(float radius, float angleInRadians)
{
  return juce::Point<float>(radius * std::cos(angleInRadians),
                            radius * std::sin(angleInRadians));
}

static inline float
degreeToRadians(float angleInDegree)
{
  return angleInDegree * (juce::MathConstants<float>::pi / 180.0f);
}

static inline float
radiansToDegree(float angleInRadians)
{
  return angleInRadians * (180.0f / juce::MathConstants<float>::pi);
}

//==============================================================================
} // namespace Math
} // namespace dmt