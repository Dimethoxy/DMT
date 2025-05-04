#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {

//==============================================================================
// Add a non-template base interface for RTTI
struct IScaleable
{
  virtual ~IScaleable() = default;
};

//==============================================================================
/**
 *  @brief This is an extension of the JUCE Component class via the CRTP idiom.
 */
template<typename Derived>
class Scaleable : public IScaleable
{
public:
  //============================================================================
  // Helper Variables
  const float& size;
  const float& scale;

  //============================================================================
  Scaleable()
    : size(fallBackSize)
    , scale(getScaleFactor())
  {
  }

private:
  //============================================================================
  inline Derived* getSelf() noexcept { return static_cast<Derived*>(this); }

  //============================================================================
  const float& getScaleFactor() noexcept
  {
    static float tempScale = getSelf()->getDesktopScaleFactor();
    if (OS_IS_DARWIN)
      tempScale *= 2.0f;
    return tempScale;
  }

private:
  const float fallBackSize = 1.0f;
  const float fallBackScale = 1.0f;
};
} // namespace dmt