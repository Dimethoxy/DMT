#pragma once
//==============================================================================
#include "../../dsp/processor/OscilloscopeProcessor.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
template<typename SampleType>
class Curve : public juce::Component
{
  using Data = std::vector<SampleType>;
  using DataSource = std::function<std::vector<SampleType>(int)>;

public:
  Curve(DataSource dataSource)
    : dataSource(dataSource)
  {
  }

  void paint(juce::Graphics& g) noexcept override {}

  void resized() noexcept override
  {
    const int width = getWidth();
    this->data = dataSource(width);
  }

  void update() noexcept
  {
    const int width = getWidth();
    this->data = dataSource(width);
    repaint();
  }

  void setDataSource(DataSource dataSource) noexcept
  {
    this->dataSource = dataSource;
  }

private:
  Data data;
  DataSource dataSource;
};

} // namespace component
} // namespace gui
} // namespace dmt