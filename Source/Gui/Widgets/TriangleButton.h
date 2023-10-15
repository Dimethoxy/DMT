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
class TriangleButton
  : public juce::Component
  , public juce::Button::Listener
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
  {
  }

  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    g.setColour(Settings::Colours::foreground);
    g.drawRect(bounds);
  }
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
      case left:
        juce::Point<float> p1(bounds.getX(), bounds.getCentreY());
        juce::Point<float> p2(bounds.getRight(), bounds.getY());
        juce::Point<float> p3(bounds.getRight(), bounds.getBottom());
        path.addTriangle(p1, p2, p3);
        break;
      case right:
        juce::Point<float> p4(bounds.getX(), bounds.getY());
        juce::Point<float> p5(bounds.getX(), bounds.getBottom());
        juce::Point<float> p6(bounds.getRight(), bounds.getCentreY());
        path.addTriangle(p4, p5, p6);
        break;
      case top:
        juce::Point<float> p7(bounds.getX(), bounds.getY());
        juce::Point<float> p8(bounds.getRight(), bounds.getY());
        juce::Point<float> p9(bounds.getCentreX(), bounds.getBottom());
        path.addTriangle(p7, p8, p9);
        break;
      case down:
        juce::Point<float> p10(bounds.getX(), bounds.getY());
        juce::Point<float> p11(bounds.getRight(), bounds.getY());
        juce::Point<float> p12(bounds.getCentreX(), bounds.getBottom());
        path.addTriangle(p10, p11, p12);
        break;
    }
    return path;
  }
  //============================================================================
private:
  Direction direction;
};
//==============================================================================
} // namespace widgets
} // namespace gui
} // namespace dmt