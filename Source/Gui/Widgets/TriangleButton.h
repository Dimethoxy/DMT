/*
  ==============================================================================

	TriangleButton.h
	Created: 15 Oct 2023 2:48:54am
	Author:  Lunix

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TriangleButton
  : public juce::Component
  , public juce::Button::Listener
{
public:
  enum Direction
  {
	left, 
	right
  };
  TriangleButton(Direction d)
	: direction(d)
  {
	addMouseListener(this, true);
  }
  

protected:
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
	}
	return path;
  }

private: 
	Direction direction;
	
};