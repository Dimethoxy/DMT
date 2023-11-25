////==============================================================================
//
//#include "ArcButtonComponent.h"
//
////==============================================================================
//namespace dmt {
//namespace gui {
//namespace components {
////==============================================================================
//ArcButtonComponent::ArcButtonComponent(bool leftSided) noexcept
//{
//  this->leftSided = leftSided;
//  outerShadow.radius = Settings::outerShadowRadius;
//  outerShadow.colour = Settings::outerShadowColour;
//  innerShadow.radius = Settings::innerShadowRadius;
//  innerShadow.colour = Settings::innerShadowColour;
//}
////==============================================================================
//void
//ArcButtonComponent::paint(juce::Graphics& g) noexcept
//{
//  juce::Path path = getPath();
//
//  // Draw the background.
//  if (hover) {
//    g.setColour(Settings::hoverColour);
//  } else {
//    g.setColour(Settings::foregroundColour);
//  }
//  g.fillPath(path);
//
//  // Draw the shadows.
//  if (Settings::drawInnerShadow)
//    innerShadow.drawInnerForPath(g, path);
//  if (Settings::drawOuterShadow)
//    outerShadow.drawOuterForPath(g, path);
//
//  // Draw the border.
//  g.setColour(Settings::borderColour);
//  g.strokePath(path, juce::PathStrokeType(Settings::borderSize));
//}
////==============================================================================
//void
//ArcButtonComponent::resized() noexcept
//{
//  auto bounds = this->getLocalBounds().toFloat();
//
//  // Reduce the bounds by 1/3.75 the width and 1/5.5 the height
//  bounds.reduce(bounds.getWidth() / 3.75f, bounds.getHeight() / 5.5f);
//
//  // If the button is clicked, reduce the bounds by 10
//  if (!clicked)
//    bounds = bounds.reduced(10.0f);
//
//  // If the button is left sided, offset the bounds to the left.
//  // Otherwise, offset the bounds to the right.
//  auto offsetDirection = leftSided ? -1.0f : 1.0f;
//  auto xOffset = bounds.getWidth() / 8.0f * offsetDirection;
//  bounds.setX(bounds.getX() + xOffset);
//
//  // Set the pathBounds to the bounds, rounded to the nearest integer
//  pathBounds = bounds.toNearestInt();
//}
////==============================================================================
//void
//ArcButtonComponent::mouseEnter(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::mouseExit(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::mouseDown(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::mouseUp(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::mouseMove(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::mouseDrag(const juce::MouseEvent& event) noexcept
//{
//  updateState(event);
//}
////==============================================================================
//void
//ArcButtonComponent::updateState(const juce::MouseEvent& event) noexcept
//{
//  // Update hover and clicked states
//  juce::Path path = getPath();
//  if (path.contains(event.getPosition().toFloat())) {
//    hover = true;
//    if (isMouseButtonDown()) {
//      clicked = true;
//    } else {
//      clicked = false;
//    }
//  } else {
//    hover = false;
//  }
//
//  // Update the component
//  resized();
//  repaint();
//}
////==============================================================================
//const juce::Path
//ArcButtonComponent::getPath() const noexcept
//{
//  juce::Path path;
//  const auto bounds = pathBounds.toFloat();
//
//  // Get the midpoint of the path.
//  float midX = leftSided ? bounds.getX() : bounds.getRight();
//  float midY = bounds.getCentreY();
//  juce::Point<float> mid(midX, midY);
//  path.startNewSubPath(mid);
//
//  // Get the top of the path.
//  int topX = leftSided ? bounds.getRight() : bounds.getX();
//  int topY = bounds.getY();
//  juce::Point<float> top(topX, topY);
//  path.lineTo(top);
//
//  // Get the bottom of the path.
//  float downX = leftSided ? bounds.getRight() : bounds.getX();
//  float downY = bounds.getY() + bounds.getHeight();
//  juce::Point<float> down(downX, downY);
//  path.quadraticTo(bounds.getCentre(), down);
//
//  // Close the path.
//  path.closeSubPath();
//  return path;
//}
////==============================================================================
//} // namespace dmt
//} // namespace gui
//} // namespace components