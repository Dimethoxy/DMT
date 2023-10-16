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
  const float& size = Settings::size;
  const float& margin = Settings::Layout::margin;
  const bool& drawOuterShadow = Settings::TriangleButton::drawOuterShadow;
  const bool& drawInnerShadow = Settings::TriangleButton::drawInnerShadow;
  const float& shadowRadius = Settings::Appearance::shadowRadius;
  const float& buttonMargin = Settings::TriangleButton::margin;
  const float& toggleReduction = Settings::TriangleButton::margin;
  const juce::Colour standbyColour = Settings::Colours::foreground;
  const juce::Colour hoverColour = Settings::Colours::primary;

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
    outerShadow.radius = shadowRadius;
    outerShadow.colour = Settings::Colours::outerShadow;
    innerShadow.radius = shadowRadius;
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
    const auto bigBounds = bounds.reduced(buttonMargin * size);
    juce::Path trianglePath;

    // Calculate clicked path
    if (isMouseButtonDown()) {
      trianglePath = getPath(bigBounds.toFloat());
    } else {
      auto smallBounds = bigBounds;
      smallBounds.setHeight(smallBounds.getHeight() * toggleReduction);
      smallBounds.setWidth(smallBounds.getWidth() * toggleReduction);
      smallBounds.setCentre(bigBounds.getCentre());
      trianglePath = getPath(smallBounds.toFloat());
    }

    // Draw outer shadow
    if (drawOuterShadow) {
      outerShadow.drawOuterForPath(g, trianglePath);
    }

    // Set triangle fill color
    if (isMouseOver() && !isMouseButtonDown())
      g.setColour(hoverColour);
    else
      g.setColour(standbyColour);

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