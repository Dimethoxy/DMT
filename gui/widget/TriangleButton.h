#pragma once

#include "../../utility/LibrarySettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class TriangleButton : public juce::Button
{
  // Global
  using LibrarySettings = dmt::LibrarySettings;
  using Settings = LibrarySettings::TriangleButton;
  const float& size = LibrarySettings::Layout::size;
  const float& margin = LibrarySettings::Layout::margin;
  // General
  const juce::Colour& standbyColour = Settings::standbyColour;
  const juce::Colour& hoverColour = Settings::hoverColour;
  const float& buttonMargin = Settings::margin;
  const float& toggleReduction = Settings::toggleReduction;
  // Border
  const bool& drawBorder = Settings::drawBorder;
  const juce::Colour& borderColour = Settings::borderColour;
  const float& rawBorderStrength = Settings::borderStrength;
  // Shadows
  const bool& drawOuterShadow = Settings::drawOuterShadow;
  const bool& drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour& outerShadowColour = Settings::outerShadowColour;
  const juce::Colour& innerShadowColour = Settings::innerShadowColour;
  const float& outerShadowRadius = Settings::outerShadowRadius;
  const float& innerShadowRadius = Settings::innerShadowRadius;

public:
  //============================================================================
  enum Direction
  {
    Left,
    Right,
    Top,
    Down
  };
  //============================================================================
  TriangleButton(Direction d)
    : direction(d)
    , juce::Button("TriangleButton")
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)

  {
  }

protected:
  juce::Path getPath(juce::Rectangle<int> bounds)
  {
    juce::Path path;
    const float left = (float)bounds.getX();
    const float right = (float)bounds.getRight();
    const float top = (float)bounds.getY();
    const float bottom = (float)bounds.getBottom();
    const float centreX = (float)bounds.getCentreX();
    const float centreY = (float)bounds.getCentreY();
    switch (direction) {
      case Direction::Left: {
        juce::Point<float> p1(left, centreY);
        juce::Point<float> p2(right, top);
        juce::Point<float> p3(right, bottom);
        path.addTriangle(p1, p2, p3);
        break;
      }
      case Direction::Right: {
        juce::Point<float> p1(left, top);
        juce::Point<float> p2(left, bottom);
        juce::Point<float> p3(right, centreY);
        path.addTriangle(p1, p2, p3);
        break;
      }
      case Direction::Top: {
        juce::Point<float> p1(left, top);
        juce::Point<float> p2(right, top);
        juce::Point<float> p3(centreX, bottom);
        path.addTriangle(p1, p2, p3);
        break;
      }
      case Direction::Down: {
        juce::Point<float> p1(left, top);
        juce::Point<float> p2(right, top);
        juce::Point<float> p3(centreX, bottom);
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
      float width = (float)bounds.getWidth();
      float height = (float)bounds.getHeight();
      float ratio = width / height;
      float borderStrength = rawBorderStrength * size;
      int reducedWidth = (int)(bounds.getWidth() - borderStrength);
      int reducedHeight = (int)(bounds.getHeight() - (borderStrength / ratio));
      bounds.setSize(reducedWidth, reducedHeight);
      bounds.setCentre(origin.getCentre());
      return getPath(bounds);
    } else
      return juce::Path();
  }

  void buttonStateChanged() { repaint(); }
  void paintButton(juce::Graphics& g,
                   bool /*shouldDrawButtonAsHighlighted*/,
                   bool /*shouldDrawButtonAsDown*/) override
  {
    const auto bounds = this->getLocalBounds();
    const int bigBoundsPadding = (int)(buttonMargin * size);
    const auto bigBounds = bounds.reduced(bigBoundsPadding);
    juce::Path outerTrianglePath;
    juce::Path innerTrianglePath;
    juce::Path& trianglePath = outerTrianglePath;

    // Calculate clicked path
    if (isMouseButtonDown()) {
      outerTrianglePath = getPath(bigBounds);
      innerTrianglePath = getTnnerTrianglePath(bigBounds);
    } else {
      auto smallBounds = bigBounds;
      const int smallWeight = (int)(smallBounds.getWidth() * toggleReduction);
      smallBounds.setWidth(smallWeight);
      const int smallHeight = (int)(smallBounds.getHeight() * toggleReduction);
      smallBounds.setHeight(smallHeight);
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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriangleButton)
};
//==============================================================================
} // namespace widgets
} // namespace gui
} // namespace dmt