#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {

//==============================================================================
/**
 *  @brief This is an extension of the JUCE Component class via the CRTP idiom.
 */
template<typename Derived>
class Scaleable
{
public:
  static inline float baseHeight = 100.0f;
  static inline float baseWidth = 100.0f;

protected:
  //==============================================================================
  // General
  const float& size = Settings::Window::size;

  inline float getScaleFactor() noexcept
  {
    const auto scale = getSelf()->getDesktopScaleFactor();
    if (OS_IS_DARWIN)
      return scale * 2.0f;
    else
      return scale;
  }

  inline float getSizeFactor() noexcept
  {
    const auto* topLevelComponent = getSelf()->getTopLevelComponent();
    return 1.0f;
  }

private:
  inline Derived* getSelf() noexcept { return static_cast<Derived*>(this); }
};
} // namespace dmt