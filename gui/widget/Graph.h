#pragma once
//==============================================================================
#include "utility/RepaintTimer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
template<typename SampleType>
class Graph
  : public juce::Component
  , public dmt::utility::RepaintTimer
{
  using DataSource = std::function<SampleType(int)>;

public:
  //==============================================================================
  Graph(DataSource dataSource, bool continuousRepainting = true)
    : dataSource(dataSource)
    , continuousRepainting(continuousRepainting)
  {
    if (continuousRepainting) {
      startRepaintTimer();
    }
  }
  //==============================================================================
  void resized() override {}
  //==============================================================================
  void paint(juce::Graphics& g) noexcept override
  {
    int width = getWidth();
    int height = getHeight();

    juce::Path path;
    path.preallocateSpace((3 * width) + 6);
    path.startNewSubPath(0, (float)height / 2.0f);

    for (int i = 0; i < width; ++i) {
      const SampleType sample = dataSource(i);
      path.lineTo((float)i,
                  (float)height / 2.0f - sample * (float)height / 2.0f);
    }

    path.lineTo((float)width, (float)height / 2.0f);
    path.closeSubPath();

    g.setColour(juce::Colours::green.withAlpha(0.5f));
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(3.0f));
  }
  //==============================================================================
private:
  DataSource dataSource;
  bool continuousRepainting;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Graph)
};
} // namespace widget
} // namespace gui
} // namespace dmt