#pragma once

#include "../Utility/AppSettings.h"
#include "../Utility/Shadow.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
/**
 * @class ArcButtonComponent
 *
 * @brief Represents a custom arc-shaped button component.
 *
 * The ArcButtonComponent class is a custom component that displays an
 * arc-shaped button. It inherits from the juce::Component class and implements
 * the juce::MouseListener interface. The button can be left-sided or
 * right-sided based on the constructor parameter.
 */
class ArcButtonComponent
  : public juce::Component
  , juce::MouseListener
{
public:
  /**
   * @brief Alias for the settings related to the ArcButtonComponent.
   *
   * The Settings alias is used to refer to the global settings specific to the
   * ArcButtonComponent. It is derived from the dmt::AppSettings::ArcButton
   * namespace.
   *
   * @param leftSided If true the arc will point to the left, otherwise it will
   * point to the right
   */
  using Settings = dmt::AppSettings::ArcButton;

  /*
   * @brief Constructs an ArcButtonComponent object.
   *
   * @param leftSided Determines whether the button is left-sided or
   * right-sided.
   */
  ArcButtonComponent(bool leftSided)
  {
    this->leftSided = leftSided;
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
  }

  /**
   * @copydoc juce::MouseListener::mouseEnter()
   */
  void mouseEnter(const juce::MouseEvent& event) override
  {

    updateState(event);
  }

  /**
   * @copydoc juce::MouseListener::mouseEnter()
   */
  void mouseExit(const juce::MouseEvent& event) override { updateState(event); }

  /**
   * @copydoc juce::MouseListener::mouseDown()
   */
  void mouseDown(const juce::MouseEvent& event) override { updateState(event); }

  /**
   * @copydoc juce::MouseListener::mouseUp()
   */
  void mouseUp(const juce::MouseEvent& event) override { updateState(event); }

  /**
   * @copydoc juce::MouseListener::mouseMove()
   */
  void mouseMove(const juce::MouseEvent& event) override { updateState(event); }

  /**
   * @copydoc juce::MouseListener::mouseDrag()
   */
  void mouseDrag(const juce::MouseEvent& event) override { updateState(event); }

  void updateState(const juce::MouseEvent& event)
  {
    juce::Path path = getPath();
    if (path.contains(event.getPosition().toFloat())) {
      hover = true;
      if (isMouseButtonDown()) {
        clicked = true;
      } else {
        clicked = false;
      }
    } else {
      hover = false;
    }
    resized();
    repaint();
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    juce::Path path = getPath();

    if (hover) {
      g.setColour(Settings::hoverColour);
    } else {
      g.setColour(Settings::foregroundColour);
    }

    g.fillPath(path);

    if (Settings::drawInnerShadow)
      innerShadow.drawInnerForPath(g, path);
    if (Settings::drawOuterShadow)
      outerShadow.drawOuterForPath(g, path);

    g.setColour(Settings::borderColour);
    g.strokePath(path, juce::PathStrokeType(Settings::borderSize));
  }
  void resized()
  {
    auto bounds = this->getLocalBounds().toFloat();
    bounds.reduce(bounds.getWidth() / 3.75f, bounds.getHeight() / 5.5f);
    if (!clicked)
      bounds = bounds.reduced(10.0f);

    auto offsetDirection = leftSided ? -1.0f : 1.0f;
    auto xOffset = bounds.getWidth() / 8.0f * offsetDirection;
    bounds.setX(bounds.getX() + xOffset);
    pathBounds = bounds.toNearestInt();
  }
  //============================================================================
  juce::Path getPath()
  {
    const auto bounds = pathBounds.toFloat();
    juce::Path path;

    float midX = leftSided ? bounds.getX() : bounds.getRight();
    float midY = bounds.getCentreY();
    juce::Point<float> mid(midX, midY);
    path.startNewSubPath(mid);

    int topX = leftSided ? bounds.getRight() : bounds.getX();
    int topY = bounds.getY();
    juce::Point<float> top(topX, topY);
    path.lineTo(top);

    float downX = leftSided ? bounds.getRight() : bounds.getX();
    float downY = bounds.getY() + bounds.getHeight();
    juce::Point<float> down(downX, downY);
    path.quadraticTo(bounds.getCentre(), down);

    path.closeSubPath();
    return path;
  }
  //============================================================================
private:
  bool hover = false;
  bool clicked = false;
  bool leftSided;
  juce::Rectangle<int> pathBounds;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
}