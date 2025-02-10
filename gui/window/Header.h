#pragma once

#include <JuceHeader.h>
namespace dmt {
namespace gui {
namespace window {
class Header : public juce::Component
{
  const float& size = dmt::Settings::Window::size;
  const float& rawBorderStrength = dmt::Settings::Panel::borderStrength;
  const juce::Colour& backroundColour = dmt::Settings::Header::backroundColour;
  const juce::Colour& borderColor = dmt::Settings::Header::borderColor;

public:
  Header()
  {
    // Constructor code here
  }

  ~Header() override
  {
    // Destructor code here
  }

  void paint(juce::Graphics& g) override
  {
    // Paint the background
    auto bounds = getLocalBounds();
    g.setColour(backroundColour);
    g.fillRect(bounds);
    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);
    g.setColour(borderColor);
    g.fillRect(bounds);
  }

  void resized() override
  {
    // Resizing code here
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt