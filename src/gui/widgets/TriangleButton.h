/*
  ==============================================================================

    TriangleButton.h
    Created: 15 Oct 2023 2:48:54am
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../../utility/AppSettings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace widgets {
//==============================================================================
class TriangleButton : public juce::Button {
  // Global
  using Settings = dmt::AppSettings::TriangleButton;
  const float &size = dmt::AppSettings::size;
  const float &margin = dmt::AppSettings::Layout::margin;
  // General
  const juce::Colour &standbyColour = Settings::standbyColour;
  const juce::Colour &hoverColour = Settings::hoverColour;
  const float &buttonMargin = Settings::margin;
  const float &toggleReduction = Settings::toggleReduction;
  // Border
  const bool &drawBorder = Settings::drawBorder;
  const juce::Colour &borderColour = Settings::borderColour;
  const float &borderStrength = Settings::borderStrength;
  // Shadows
  const bool &drawOuterShadow = Settings::drawOuterShadow;
  const bool &drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour &outerShadowColour = Settings::outerShadowColour;
  const juce::Colour &innerShadowColour = Settings::innerShadowColour;
  const float &outerShadowRadius = Settings::outerShadowRadius;
  const float &innerShadowRadius = Settings::innerShadowRadius;

public:
  //============================================================================
  enum Direction { left, right, top, down };
  //============================================================================
  TriangleButton(Direction d)
      : direction(d), juce::Button("TriangleButton"),
        outerShadow(outerShadowColour, outerShadowRadius),
        innerShadow(innerShadowColour, innerShadowRadius)

  {}

protected:
  juce::Rectangle<int> getTriangleBounds(const juce::Rectangle<int> bounds) {
    juce::Rectangle<int> result;
    return result;
  }
  juce::Path getPath(juce::Rectangle<int> bounds) {
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
  juce::Path getTnnerTrianglePath(juce::Rectangle<int> origin) {
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
  void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override {
    const auto bounds = this->getLocalBounds();
    const auto bigBounds = bounds.reduced(buttonMargin * size).toNearestInt();
    juce::Path outerTrianglePath;
    juce::Path innerTrianglePath;
    juce::Path &trianglePath = outerTrianglePath;

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

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriangleButton)
};
//==============================================================================
} // namespace widgets
} // namespace gui
} // namespace dmt