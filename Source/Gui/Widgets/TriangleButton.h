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
  const float& toggleReduction = Settings::TriangleButton::toggleReduction;
  const bool& drawBorder = Settings::TriangleButton::drawBorder;
  const float& borderStrength = Settings::TriangleButton::borderStrength;
  const juce::Colour standbyColour =
    Settings::Colours::foreground.withLightness(0.5);
  const juce::Colour hoverColour = Settings::Colours::primary;
  const juce::Colour borderColour = Settings::Colours::foreground;

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
  juce::Path getPath(juce::Rectangle<int> bounds)
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
  juce::Path getTnnerTrianglePath(juce::Rectangle<int> origin)
  {
    if (drawBorder) {
      auto bounds = origin;
      float width = bounds.getWidth();
      float height = bounds.getHeight();
      float ratio = width / height;
      auto reducedWidth = bounds.getWidth() - borderStrength * size;
      auto reducedHeight = bounds.getHeight() - (borderStrength * size / ratio);
      bounds.setSize(reducedWidth, reducedHeight);
      bounds.setCentre(origin.getCentre());
      return getPath(bounds);
    } else
      return juce::Path();
  }

  void buttonStateChanged() { repaint(); }
  void paintButton(juce::Graphics& g,
                   bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override
  {
    const auto bounds = this->getLocalBounds();
    const auto bigBounds = bounds.reduced(buttonMargin * size).toNearestInt();
    juce::Path outerTrianglePath;
    juce::Path innerTrianglePath;
    juce::Path& trianglePath = outerTrianglePath;

    // Calculate clicked path
    if (isMouseButtonDown()) {
      outerTrianglePath = getPath(bigBounds);
      innerTrianglePath = getTnnerTrianglePath(bigBounds);
    } else {
      auto smallBounds = bigBounds;
      smallBounds.setHeight(smallBounds.getHeight() * toggleReduction);
      smallBounds.setWidth(smallBounds.getWidth() * toggleReduction);
      smallBounds.setCentre(bigBounds.getCentre());
      outerTrianglePath = getPath(smallBounds);
      innerTrianglePath = getTnnerTrianglePath(smallBounds);
    }

    // Draw outer shadow
    if (drawOuterShadow) {
      outerShadow.drawOuterForPath(g, outerTrianglePath);
    }

    // Draw Border
    if (drawBorder) {
      g.setColour(borderColour);
      g.fillPath(outerTrianglePath);
      trianglePath = innerTrianglePath;
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
      innerShadow.drawInnerForPath(g, outerTrianglePath);
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