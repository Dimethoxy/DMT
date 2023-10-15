//==============================================================================

#pragma once

#include "../../Utility/AppSettings.h"
#include "../../Utility/Shadow.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
/*
 How to use: 
 Panel p1, p2;
 Carousel c({ &p1, &p2 }); 
*/
class Carousel
{
public:
  Carousel(std::vector<Panel&> panels) : panels(panels) {
    for (Panel& p : panels) {
      p.setCarousel(*this);
    }
  }
  void next() { 
    panels[index].setVisible(false);
    index++;
    if (index > panels.size())
        index -= panels.size();
    panels[index].setVisible(true);

  }
  void previous() {
    panels[index].setVisible(false);
    index--;
    if (index < 0)
        index += panels.size();
    panels[index].setVisible(true);
  }
private:
  int index = 0;
  std::vector<Panel&> panels;
};


  //==============================================================================
class Panel : public juce::Component
{
  using Settings = dmt::AppSettings;
  using Colours = Settings::Colours;
  using Fonts = Settings::Fonts;
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

  void setCarousel(Carousel& c) { 
      carousel = std::make_unique<Carousel>(c);
  }

  void paint(juce::Graphics& g) override
  {
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(margin);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = cornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);
    
    // Draw outer shadow
    juce::Path outerShadowPath;
    if (drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    // Draw background if border is disabled
    if (!drawBorder) {
      g.setColour(Colours::solidDark);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(Colours::solidMid);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
      g.setColour(Colours::solidDark);
      g.fillRoundedRectangle(innerBounds, innerCornerSize);
    }

    // Draw the inner shadow
    juce::Path innerShadowPath;
    if (drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }


    //Draw the title text
    auto textBounds = innerBounds;
    g.setColour(Colours::foreground);
    auto titleFont =
      Fonts::regular.withHeight(Fonts::panelTitleSize * size); 
    g.setFont(titleFont);
    g.drawText(getName(),
               textBounds.toNearestInt(),
               juce::Justification::centredTop,
               1);

    // Draw the previous and next button
    if (carousel) {
        // Draw nextButton
        // Draw previousButton
    }
  }

  virtual juce::String getName() { return "Panel"; }

private:
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
  std::unique_ptr<Carousel> carousel;
  std::unique_ptr<Carousel> nextButton;
  std::unique_ptr<Carousel> previousButton;
};

//==============================================================================
} // namespace gui
} // namespace dmt
