#pragma once
//==============================================================================
#include "utility/RepaintTimer.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace widget {
//==============================================================================
template<typename Container>
class Graph
  : public juce::Component
  , public dmt::utility::RepaintTimer
{

  using SampleType = typename Container::value_type;

  using Data = const SampleType*;
  using DataSource = std::function<const Container*(int)>;

  static_assert(std::is_floating_point_v<SampleType>,
                "Containers SampleType must be a floating point type!");
  static_assert(std::is_same_v<decltype(std::declval<Container>()[0]),
                               typename Container::value_type&>,
                "Container must support element access via []!");

public:
  enum class Quality
  {
    Low,
    High
  };
  //==============================================================================
  Graph(DataSource dataSource,
        bool continuousRepainting = false,
        Quality quality = Quality::Low)
    : dataSource(dataSource)
    , quality(quality)
    , continuousRepainting(continuousRepainting)
  {
    if (continuousRepainting) {
      startRepaintTimer();
    }
  }

  //==============================================================================
  void paint(juce::Graphics& g) noexcept override
  {
    juce::Path path;
    const int width = getWidth();
    const int height = getHeight();
    Data data = dataSource(width);

    switch (quality) {
      case Quality::Low:
        path = makeLowQualityPath(data, width, height);
        break;
      case Quality::High:
        path = makeHighQualityPath(data, width, height);
        break;
    }

    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(3.0f));
  }

  void resized() override {}

protected:
  juce::Path makeLowQualityPath(Data data, int width, int height)
  {
    juce::Path path;
    path.preallocateSpace((3 * width) + 6);
    path.startNewSubPath(0, (float)height / 2.0f);
    for (int i = 0; i < width; ++i) {
      path.lineTo((float)i,
                  (float)height / 2.0f - data[i] * (float)height / 2.0f);
    }
    path.lineTo((float)width, (float)height / 2.0f);
    path.closeSubPath();
    return path;
  }

  juce::Path makeHighQualityPath(Data data, int width, int height)
  {
    int numPoints = width * 3;
    float pointSpacing = width / (float)(numPoints - 1);
    juce::Path path;
    path.preallocateSpace(numPoints * 3 + 6);
    path.startNewSubPath(0, (float)height / 2.0f);
    for (int i = 0; i < numPoints; ++i) {
      float x = i * pointSpacing;
      float y = height / 2 - data[i] * height / 2;
      path.lineTo(x, y);
    }
    path.lineTo((float)width, (float)height / 2.0f);
    path.closeSubPath();
    return path;
  }

  //==============================================================================
private:
  DataSource dataSource;
  Quality quality;
  bool continuousRepainting;
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Graph)
};
} // namespace widget
} // namespace gui
} // namespace dmt