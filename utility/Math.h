//==============================================================================

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace math {
//==============================================================================
static inline const float pi = juce::MathConstants<float>::pi;
static inline const float twoPi = juce::MathConstants<float>::twoPi;
static inline float
linearToExponent(float value) noexcept
{
  return (value >= 0.0f) ? (value + 1.0f) : (1.0f / -(value + 1.0f));
}

static inline const juce::Point<float>
pointOnCircle(const float radius, const float angleInRadians) noexcept
{
  return juce::Point<float>(radius * std::cos(angleInRadians - pi * 0.5f),
                            radius * std::sin(angleInRadians - pi * 0.5f));
}

static inline const juce::Point<float>
pointOnCircle(juce::Point<float> centre,
              const float radius,
              const float angleInRadians) noexcept
{
  juce::Point<float> raw(radius * std::cos(angleInRadians - pi * 0.5f),
                         radius * std::sin(angleInRadians - pi * 0.5f));
  return raw + centre;
}

static inline const float
degreeToRadians(float angleInDegree) noexcept
{
  return angleInDegree * (juce::MathConstants<float>::pi / 180.0f);
}

static inline const float
normalizeAngleInRadians(float angleInRadians) noexcept
{
  if (angleInRadians < 0.0f)
    angleInRadians += twoPi;
  return angleInRadians;
}

static inline const float
radiansToDegree(float angleInRadians) noexcept
{
  return angleInRadians * (180.0f / juce::MathConstants<float>::pi);
}

//==============================================================================
} // namespace Math
} // namespace dmt