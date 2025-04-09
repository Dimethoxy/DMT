//==============================================================================
#pragma once
//==============================================================================
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace window {
//==============================================================================
class Popover : public juce::Component
{
  using String = juce::String;
  using Point = juce::Point<int>;
  using Rectangle = juce::Rectangle<int>;

  using Settings = dmt::Settings;
  using Layout = dmt::Settings::Window;

public:
  Popover()
  {
    setAlwaysOnTop(true);
    // setVisible(false); TODO: Put this back in after testing
    setInterceptsMouseClicks(false, false);
  }
  ~Popover() override { setVisible(false); }
  void paint(juce::Graphics& g) override
  {
    g.fillAll(juce::Colours::green.withAlpha(0.2f));
    g.setColour(juce::Colours::red);
    g.fillRect(messageBounds);
  }
  void resized() override {}

  void showMessage(Point position)
  {
    messageBounds.setSize(200, 100);
    messageBounds.setCentre(position);
    messageBounds.setTop(position.y);
    repaint();
  }

private:
  Rectangle messageBounds;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Popover)
};
//==============================================================================
} // namespace components
} // namespace gui
} // namespace dmt