#pragma once

#include <JuceHeader.h>
namespace dmt {
namespace gui {
namespace component {
class HeaderComponent : public juce::Component
{
  const juce::Colour& headerColour = dmt::Settings::Header::backroundColour;

public:
  HeaderComponent()
  {
    // Constructor code here
  }

  ~HeaderComponent() override
  {
    // Destructor code here
  }

  void paint(juce::Graphics& g) override
  {
    // We gotta reduce the header bounds at the bottom to make room for the
    // shadow Left, right and top don't need to be reduced because the windows
    // border will cover it anyways.
  }

  void resized() override
  {
    // Resizing code here
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};
} // namespace component
} // namespace gui
} // namespace dmt