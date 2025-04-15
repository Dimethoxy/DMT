//==============================================================================

#pragma once

#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace math {
//==============================================================================
static inline const float pi = juce::MathConstants<float>::pi;
static inline const float twoPi = juce::MathConstants<float>::twoPi;

/**
 * Calculates the angle in radians from a given slope.
 *
 * This function computes the angle in radians from a given slope using the
 * atan function. The slope is expected to be a floating-point type.
 *
 * @param slope The slope value. It is expected to be a floating-point type
 *              (e.g., `float` or `double`).
 * @return The angle in radians corresponding to the slope.
 *
 * @tparam SlopeType The desired type of the slope. Must be a floating-point
 * type (e.g., `float`, `double`).
 *
 * @note This function uses `atan` to calculate the angle in radians.
 *
 * @warning Ensure that the SlopeType is a floating-point type. This function
 *          may result in undefined behavior if passed a non-floating-point
 * type.
 *
 * @example
 * float angle = slopeToAngleInRadians<float>(slope);
 */
template<typename SlopeType>
[[nodiscard]] static inline SlopeType
slopeToAngleInRadians(SlopeType slope) noexcept
{
  // Ensure SlopeType is a floating-point type at compile-time
  static_assert(std::is_floating_point_v<SlopeType>,
                "SlopeType must be a floating-point type.");

  // Calculate and return the angle in radians using atan
  return std::atan(slope);
}

/**
 * Calculates the slope from a given angle in radians.
 *
 * This function computes the slope from a given angle in radians using the
 * tan function. The angle is expected to be a floating-point type representing
 * the angle in radians.
 *
 * @param angle The angle in radians. It is expected to be a floating-point type
 *              (e.g., `float` or `double`).
 * @return The slope corresponding to the angle in radians.
 *
 * @tparam AngleType The desired type of the angle. Must be a floating-point
 * type (e.g., `float`, `double`).
 *
 * @note This function uses `tan` to calculate the slope from the angle.
 *
 * @warning Ensure that the AngleType is a floating-point type. This function
 *          may result in undefined behavior if passed a non-floating-point
 *          type.
 *
 * @example
 * float slope = angleInRadiansToSlope<float>(angle);
 */
template<typename AngleType>
[[nodiscard]] static inline AngleType
angleInRadiansToSlope(AngleType angle) noexcept
{
  // Ensure AngleType is a floating-point type at compile-time
  static_assert(std::is_floating_point_v<AngleType>,
                "AngleType must be a floating-point type.");

  // Calculate and return the slope using tan
  return std::tan(angle);
}

/**
 * Calculates the slope between two points.
 *
 * This function calculates the slope of the line connecting two points (a and
 * b). It returns an optional value, which will be nullopt if the slope is
 * undefined (i.e., if the line is vertical).
 *
 * @param a The first point.
 * @param b The second point.
 * @return An optional value containing the slope if defined, or nullopt if
 * undefined.
 *
 * @tparam SlopeType The desired type of the slope result. Must be a floating
 * point type.
 * @tparam PointType The type used in the point coordinates.
 *
 * @note This function uses JUCE's approximatelyEqual() to safely check for
 * division by zero.
 *
 * @warning Make sure SlopeType is a floating point type.
 *
 * @example float slope = calculateSlope<float>(pointA, pointB).value_or(0.0f);
 */
template<typename SlopeType, typename PointType>
[[nodiscard]] static inline std::optional<SlopeType>
calculateSlope(const juce::Point<PointType>& a,
               const juce::Point<PointType>& b) noexcept
{
  // Ensure SlopeType is a floating point type at compile time
  static_assert(std::is_floating_point<SlopeType>::value,
                "SlopeType must be a floating point type.");

  // Convert point coordinates to the desired slope type
  const SlopeType x1 = a.x;
  const SlopeType y1 = a.y;
  const SlopeType x2 = b.x;
  const SlopeType y2 = b.y;

  // Compute differences in X and Y
  const SlopeType deltaX = x2 - x1;
  const SlopeType deltaY = y2 - y1;

  // Avoid division by zero by checking if deltaX is approximately zero
  if (juce::approximatelyEqual(deltaX, SlopeType{ 0 })) {
    jassertfalse; // Vertical line: slope is undefined
    return std::nullopt;
  }

  // Return the calculated slope
  return deltaY / deltaX;
}

/**
 * Moves a point in a given direction by a specified distance.
 *
 * @param origin   The starting point.
 * @param angleRad The direction to move in, in radians.
 * @param distance The distance to move.
 * @return A new point representing the result of moving from the origin
 *         in the specified direction by the given distance.
 *
 * @tparam CoordType The floating-point type used for point coordinates.
 *
 * @note The direction is interpreted as a standard angle in radians,
 *       where 0 is to the right (positive X axis), and positive angles
 *       rotate counter-clockwise.
 *
 * @example
 * juce::Point<float> result = marchPoint<float>(start, angle, 10.0f);
 */
template<typename CoordType>
[[nodiscard]] static inline juce::Point<CoordType>
marchPoint(const juce::Point<CoordType>& origin,
           CoordType angleRad,
           CoordType distance) noexcept
{
  static_assert(std::is_floating_point_v<CoordType>,
                "CoordType must be a floating-point type.");

  const CoordType dx = std::cos(angleRad) * distance;
  const CoordType dy = std::sin(angleRad) * distance;

  return { origin.x + dx, origin.y + dy };
}

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