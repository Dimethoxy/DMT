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
  using Settings = dmt::AppSettings::Panel;

public:
  Panel()
    : top(getName())
  {
    addAndMakeVisible(top);
    outerShadow.radius = Settings::outerShadowRadius;
    outerShadow.colour = Settings::outerShadowColour;
    innerShadow.radius = Settings::innerShadowRadius;
    innerShadow.colour = Settings::innerShadowColour;
    resized();
  }

  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    if (Settings::drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(borderBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    g.setColour(Settings::borderColour);
    g.fillRoundedRectangle(borderBounds, outerCornerSize);

    g.setColour(Settings::foregroundColour);
    g.fillRoundedRectangle(innerBounds, innerCornerSize);

    if (Settings::drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, outerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }

  void resized() override
  {
    const auto bounds = this->getLocalBounds().toFloat();
    borderSize = Settings::borderSize;
    borderBounds = bounds.reduced(Settings::margin);
    innerBounds = borderBounds.reduced(borderSize);
    outerCornerSize = Settings::outerCornerSize;
    innerCornerSize = Settings::innerCornerSize;

    top.setBounds(innerBounds.toNearestInt());
    top.setSize(top.getWidth(), top.getHeight() * 0.3f);
    top.setAlwaysOnTop(true);
    update();
  }

  virtual void update(){};

  virtual juce::String getName() { return "Panel"; };

protected:
  float borderSize = Settings::borderSize;
  juce::Rectangle<float> borderBounds;
  juce::Rectangle<float> innerBounds;
  float outerCornerSize;
  float innerCornerSize;

private:
  dmt::gui::components::TitleTopComponent top;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
//==============================================================================
} // namespace gui
} // namespace dmt
