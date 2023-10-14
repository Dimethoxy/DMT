////==============================================================================
//
//#pragma once
//
//#include "../../Utility/AppSettings.h"
//#include "../../Utility/Shadow.h"
//#include <JuceHeader.h>
//
////==============================================================================
//namespace dmt {
//namespace gui {
//namespace components {
///**
// * @class ArcButtonComponent
// *
// * @brief Represents a custom arc-shaped button component.
// *
// * The ArcButtonComponent class is a custom component that displays an
// * arc-shaped button. It inherits from the juce::Component class and implements
// * the juce::MouseListener interface. The button can be left-sided or
// * right-sided based on the constructor parameter.
// */
//class ArcButtonComponent
//  : public juce::Component
//  , juce::MouseListener
//{
//public:
//  /**
//   * @brief Alias for the settings related to the ArcButtonComponent.
//   *
//   * The Settings alias is used to refer to the global settings specific to the
//   * ArcButtonComponent. It is derived from the dmt::AppSettings::ArcButton
//   * namespace.
//   *
//   * @see dmt::AppSettings
//   */
//  using Settings = dmt::AppSettings::ArcButton;
//
//  //============================================================================
//  /**
//   * @brief Constructs an ArcButtonComponent object.
//   *
//   * @param leftSided Determines whether the button is left-sided or
//   * right-sided.
//   */
//  ArcButtonComponent(bool leftSided) noexcept;
//  ;
//
//  //============================================================================
//  /**
//   * @brief Called when the component is being painted.
//   *
//   * The paint() function is responsible for drawing the component based on its
//   * current state.
//   *
//   * @param g The juce::Graphics object used for drawing.
//   *
//   * @see juce::Component::paint()
//   */
//  void paint(juce::Graphics& g) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the component is being resized.
//   *
//   * The resized() function updates the pathBounds member variables used for
//   * drawing the component and reduces the bounds if the button is clicked.
//   *
//   * @see juce::Component::resized()
//   */
//  void resized() noexcept override;
//
//protected:
//  //============================================================================
//  /**
//   * @brief Called when the mouse enters the component.
//   *
//   * The mouseEnter() function updates the state of the component when the mouse
//   * enters its area. It calls the updateState() function to handle state
//   * changes based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   *
//   * @see juce::MouseListener::mouseEnter()
//   */
//  void mouseEnter(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the mouse exits the component.
//   *
//   * The mouseExit() function updates the state of the component when the mouse
//   * exits its area. It calls the updateState() function to handle state changes
//   * based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   *
//   * @see juce::MouseListener::mouseExit()
//   */
//  void mouseExit(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the mouse button is pressed down.
//   *
//   * The mouseDown() function updates the state of the component when the mouse
//   * button is pressed down. It calls the updateState() function to handle state
//   * changes based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   */
//  void mouseDown(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the mouse button is released.
//   *
//   * The mouseUp() function updates the state of the component when the mouse
//   * button is released. It calls the updateState() function to handle state
//   * changes based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   */
//  void mouseUp(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the mouse is moved.
//   *
//   * The mouseMove() function updates the state of the component when the mouse
//   * is moved within its area. It calls the updateState() function to handle
//   * state changes based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   */
//  void mouseMove(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Called when the mouse is dragged.
//   *
//   * The mouseDrag() function updates the state of the component when the mouse
//   * is dragged within its area. It calls the updateState() function to handle
//   * state changes based on the mouse event.
//   *
//   * @param event The juce::MouseEvent that triggered the function.
//   */
//  void mouseDrag(const juce::MouseEvent& event) noexcept override;
//
//  //============================================================================
//  /**
//   * @brief Updates the state of the component based on the current mouse event.
//   *
//   * The updateState() function is called by the mouse event functions to update
//   * the hover and clicked states of the component. It also triggers the
//   * resized() and repaint() functions.
//   *
//   * @param event The mouse event that triggered the function.
//   */
//  void updateState(const juce::MouseEvent& event) noexcept;
//
//  //============================================================================
//  /**
//   * @brief Generates and returns the path of the component.
//   *
//   * The getPath() function is called by the paint() function to generate and
//   * retrieve the path used for drawing the component.
//   *
//   * @return The path of the component.
//   */
//  const juce::Path getPath() const noexcept;
//
//private:
//  //============================================================================
//  bool hover = false;
//  bool clicked = false;
//  bool leftSided;
//  //============================================================================
//  juce::Rectangle<int> pathBounds;
//  //============================================================================
//  dmt::Shadow outerShadow;
//  dmt::Shadow innerShadow;
//  //============================================================================
//};
//} // namespace components
//} // namespace gui
//} // namespace dmt