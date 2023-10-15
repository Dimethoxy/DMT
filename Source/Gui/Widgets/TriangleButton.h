/*
  ==============================================================================

    TriangleButton.h
    Created: 15 Oct 2023 2:48:54am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../../Utility/AppSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class TriangleButton : public juce::Button
{
  using Settings = dmt::AppSettings;

public:
  //============================================================================
  enum Direction
  {
    left,
    right,
    top,
    down
  };
  //============================================================================
  TriangleButton(Direction d)
    : direction(d)
    , juce::Button("TriangleButton")
  {
  }

  // void paint(juce::Graphics& g) override
  //{
  //   const auto bounds = this->getLocalBounds();
  //   g.setColour(Settings::Colours::foreground);
  //   g.drawRect(bounds);
  // }
  //============================================================================
protected:
  juce::Rectangle<int> getTriangleBounds(const juce::Rectangle<int> bounds)
  {
    juce::Rectangle<int> result;
    return result;
  }
  juce::Path getPath(juce::Rectangle<float> bounds)
  {
    juce::Path path;
    switch (direction) {
      case left: {
        juce::Point<float> p1(bounds.getX(), bounds.getCentreY());
        juce::Point<float> p2(bounds.getRight(), bounds.getY());
        juce::Point<float> p3(bounds.getRight(), bounds.getBottom());
        path.addTriangle(p1, p2, p3);
        break;
      }
      case right: {
        juce::Point<float> p1(bounds.getX(), bounds.getY());
        juce::Point<float> p2(bounds.getX(), bounds.getBottom());
        juce::Point<float> p3(bounds.getRight(), bounds.getCentreY());
        path.addTriangle(p1, p2, p3);
        break;
      }
      case top: {
        juce::Point<float> p1(bounds.getX(), bounds.getY());
        juce::Point<float> p2(bounds.getRight(), bounds.getY());
        juce::Point<float> p3(bounds.getCentreX(), bounds.getBottom());
        path.addTriangle(p1, p2, p3);
        break;
      }
      case down: {
        juce::Point<float> p1(bounds.getX(), bounds.getY());
        juce::Point<float> p2(bounds.getRight(), bounds.getY());
        juce::Point<float> p3(bounds.getCentreX(), bounds.getBottom());
        path.addTriangle(p1, p2, p3);
        break;
      }
    }
    return path;
  }
  void paintButton(juce::Graphics& g,
                   bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override
  {
    const auto bounds = this->getLocalBounds();
    g.setColour(Settings::Colours::foreground);
    g.drawRect(bounds);
  }
  //============================================================================
private:
  Direction direction;
};
//==============================================================================
} // namespace widgets
} // namespace gui
} // namespace dmt