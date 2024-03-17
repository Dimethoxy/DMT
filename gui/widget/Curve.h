//==============================================================================
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
class Curve
  : public juce::Component
  , public dmt::utility::RepaintTimer
{
  using Data = std::vector<SampleType>;
  using DataSource = std::function<std::vector<SampleType>(int)>;

public:
  Curve(DataSource dataSource)
    : dataSource(dataSource)
  {
    startRepaintTimer();
  }

  void paint(juce::Graphics& g) noexcept override
  {
    const int width = getWidth();
    const int height = getHeight();
    this->data = dataSource(width);

    juce::Path path;
    path.startNewSubPath(0, height / 2);

    for (int i = 0; i < width; i += 2) {
      path.lineTo(i, height / 2 - this->data[i] * height / 2);
      path.lineTo(i, height / 2 - this->data[i + 1] * height / 2);
    }
    path.lineTo(width, height / 2);
    // path.closeSubPath();

    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(3.0f));
  }

  void setDataSource(DataSource dataSource) noexcept
  {
    this->dataSource = dataSource;
  }

  void repaintTimerCallback() override { repaint(); }

private:
  Data data;
  DataSource dataSource;
};

} // namespace component
} // namespace gui
} // namespace dmt