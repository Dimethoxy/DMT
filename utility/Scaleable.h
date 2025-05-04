#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
//==============================================================================
class SizeServer
{
public:
  virtual ~SizeServer() = default;
  virtual float& getSizeFactor() const = 0;
};
//==============================================================================
/**
 *  @brief This is an extension of the JUCE Component class via the CRTP idiom.
 */
template<typename Derived>
class Scaleable
{
protected:
  //============================================================================
  // Helper Variables
  const float& size;
  const float& scale;

  //============================================================================
  Scaleable()
    : size(getSizeFactor())
    , scale(getScaleFactor())
  {
  }

private:
  //============================================================================
  inline Derived* getSelf() noexcept { return static_cast<Derived*>(this); }

  //============================================================================
  inline float& getSizeFactor() noexcept { return dmt::Settings::Window::size; }

  //============================================================================
  inline const float& getScaleFactor() noexcept
  {
    static float tempScale = getSelf()->getDesktopScaleFactor();
    if (OS_IS_DARWIN)
      tempScale *= 2.0f;
    return tempScale;
  }
};
} // namespace dmt