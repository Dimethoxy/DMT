#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
class Scaleable
{
protected:
  //==============================================================================
  // General
  const float& size = Settings::Window::size;

  const float getScaleFactor(juce::Component* _component) noexcept
  {
    const float scale = _component->getDesktopScaleFactor();
    if (OS_IS_DARWIN)
      return scale * 2.0f;
    else
      return scale;
  }
};
} // namespace dmt