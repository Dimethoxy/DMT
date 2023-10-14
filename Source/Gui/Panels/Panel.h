//==============================================================================

#pragma once

#include "../../Utility/AppSettings.h"
#include "../../Utility/Shadow.h"
#include "../Components/TitleTopComponent.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
//==============================================================================
class Panel : public juce::Component
{
  using Settings = dmt::AppSettings;
  using Colours = Settings::Colours;
  const float& size = Settings::size;
  const float& margin = Settings::Layout::margin;
  const bool& drawOuterShadow = Settings::Appearance::drawOuterShadow;
  const bool& drawInnerShadow = Settings::Appearance::drawInnerShadow;
  const bool& drawBorder = Settings::Appearance::drawBorder;
  const float& borderStrength = Settings::Appearance::borderStrength;
  const float& cornerSize = Settings::Appearance::cornerSize;

public:
  Panel()
  {
	outerShadow.radius = Settings::Layout::margin;
	outerShadow.colour = Settings::Colours::outerShadow;
	innerShadow.radius = Settings::Layout::margin;
	innerShadow.colour = Settings::Colours::innerShadow;
	resized();
  }

  void paint(juce::Graphics& g) override
  {
	const auto bounds = this->getLocalBounds().toFloat();
	const auto outerBounds = bounds.reduced(margin);
	const auto innerBounds = outerBounds.reduced(margin*size);
	const auto outerCornerSize = cornerSize * size;
	const auto innerCornerSize = outerCornerSize - (borderStrength * size);
	
	juce::Path outerShadowPath;
	if (drawOuterShadow) {
	  juce::Path outerShadowPath;
	  outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
	  outerShadow.drawOuterForPath(g, outerShadowPath);
	}

	g.setColour(Colours::foreground.withAlpha(0.1f));
	g.fillRoundedRectangle(outerBounds, outerCornerSize);

	if (drawBorder) {
	  g.setColour(Colours::background);
	  g.fillRoundedRectangle(innerBounds, innerCornerSize);

	  g.setColour(Colours::foreground.withAlpha(0.1f));
	  g.fillRoundedRectangle(outerBounds, outerCornerSize);
	}

	juce::Path innerShadowPath;
	if (drawInnerShadow) {
	  juce::Path innerShadowPath;
	  innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
	  innerShadow.drawInnerForPath(g, innerShadowPath);
	}
  }

  virtual juce::String getName() { return "Panel"; }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
//==============================================================================
} // namespace gui
} // namespace dmt
