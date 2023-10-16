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
  using Carousel = Settings::Carousel;
  const float& size = Settings::size;
  const float& margin = Settings::Layout::margin;
  const bool& drawOuterShadow = Settings::Appearance::drawOuterShadow;
  const bool& drawInnerShadow = Settings::Appearance::drawInnerShadow;

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
    outerShadow.radius = Settings::Appearance::shadowRadius;
    outerShadow.colour = Settings::Colours::outerShadow;
    innerShadow.radius = Settings::Appearance::shadowRadius;
    innerShadow.colour = Settings::Colours::innerShadow;
    resized();
  }

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
  void buttonStateChanged() { repaint(); }
  void paintButton(juce::Graphics& g,
                   bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override
  {
    const auto bounds = this->getLocalBounds();
    const auto bigBounds = bounds.reduced(Carousel::buttonMargin * size);
    juce::Path trianglePath;

    // Calculate clicked path
    if (isMouseButtonDown()) {
      trianglePath = getPath(bigBounds.toFloat());
    } else {
      auto smallBounds = bigBounds;
      smallBounds.setHeight(smallBounds.getHeight() *
                            Carousel::toggleReduction);
      smallBounds.setWidth(smallBounds.getWidth() * Carousel::toggleReduction);
      smallBounds.setCentre(bigBounds.getCentre());
      trianglePath = getPath(smallBounds.toFloat());
    }

    // Draw outer shadow
    if (drawOuterShadow) {
      outerShadow.drawOuterForPath(g, trianglePath);
    }

    // Set triangle fill color
    if (isMouseOver() && !isMouseButtonDown())
      g.setColour(Settings::Colours::primary);
    else
      g.setColour(Settings::Colours::foreground);

    // Fill triangle
    g.fillPath(trianglePath);

    // Draw inner shadow
    if (drawInnerShadow) {
      innerShadow.drawInnerForPath(g, trianglePath);
    }
  }
  //============================================================================
private:
  Direction direction;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
//==============================================================================
} // namespace widgets
} // namespace gui
} // namespace dmt