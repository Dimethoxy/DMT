#pragma once
//==============================================================================
#include <JuceHeader.h>
#include <theme/StyleSheet.h>
#include <utility/Settings.h>
//==============================================================================
namespace dmt {
namespace theme {
//==============================================================================
class Loader
{
  using Settings = dmt::Settings;
  using Panel = Settings::Panel;
  using Slider = Settings::Slider;

public:
  //============================================================================
  Loader() = delete;
  //============================================================================
  Loader(const Loader& obj) = delete;
  //============================================================================
  ~Loader() = delete;

protected:
  //============================================================================
  void applyTheme()
  {
    applyPanel();
    applySlider();
  }
  //============================================================================
  void applyPanel()
  {
    // General
    sheet.applyFloat(Panel::padding, "Panel", "padding");
    sheet.applyFloat(Panel::cornerSize, "Panel", "cornerSize");
    // Border
    sheet.applyBool(Panel::drawBorder, "Panel", "drawBorder");
    sheet.applyColour(Panel::borderColour, "Panel", "borderColour");
    sheet.applyFloat(Panel::borderStrength, "Panel", "borderStrength");
    sheet.applyColour(Panel::backgroundColour, "Panel", "backgroundColour");
    // Shadows
    sheet.applyBool(Panel::drawOuterShadow, "Panel", "drawOuterShadow");
    sheet.applyBool(Panel::drawInnerShadow, "Panel", "drawInnerShadow");
    sheet.applyColour(Panel::outerShadowColour, "Panel", "outerShadowColour");
    sheet.applyColour(Panel::innerShadowColour, "Panel", "innerShadowColour");
    sheet.applyFloat(Panel::outerShadowRadius, "Panel", "outerShadowRadius");
    sheet.applyFloat(Panel::innerShadowRadius, "Panel", "innerShadowRadius");
    // Text
    sheet.applyColour(Panel::fontColor, "Panel", "fontColor");
    sheet.applyFloat(Panel::fontSize, "Panel", "fontSize");
  }
  //============================================================================
  void applySlider()
  {
    // General
    sheet.applyFloat(Slider::padding, "Slider", "padding");
    sheet.applyFloat(Slider::baseWidth, "Slider", "baseWidth");
    sheet.applyFloat(Slider::baseHeight, "Slider", "baseHeight");
    sheet.applyFloat(Slider::sliderSize, "Slider", "sliderSize");
    sheet.applyFloat(Slider::labelsSize, "Slider", "labelsSize");
    // Font
    sheet.applyColour(Slider::titleFontColour, "Slider", "titleFontColour");
    sheet.applyColour(Slider::infoFontColour, "Slider", "infoFontColour");
    sheet.applyFloat(Slider::titleFontSize, "Slider", "titleFontSize");
    sheet.applyFloat(Slider::infoFontSize, "Slider", "infoFontSize");
    // Shaft
    sheet.applyColour(Slider::shaftColour, "Slider", "shaftColour");
    sheet.applyFloat(Slider::shaftLineStrength, "Slider", "shaftLineStrength");
    sheet.applyFloat(Slider::shaftSize, "Slider", "shaftSize");
    // Rail
    sheet.applyColour(Slider::lowerRailColour, "Slider", "lowerRailColour");
    sheet.applyColour(Slider::upperRailColour, "Slider", "upperRailColour");
    sheet.applyFloat(Slider::railWidth, "Slider", "railWidth");
    sheet.applyFloat(Slider::railSize, "Slider", "railSize");
    // Thumb
    sheet.applyColour(Slider::thumbInnerColour, "Slider", "thumbInnerColour");
    sheet.applyColour(Slider::thumOuterColour, "Slider", "thumOuterColour");
    sheet.applyFloat(Slider::thumbSize, "Slider", "thumbSize");
    sheet.applyFloat(Slider::thumbStrength, "Slider", "thumbStrength");
    // Selections
    sheet.applyColour(
      Slider::selectionOuterColour, "Slider", "selectionOuterColour");
    sheet.applyColour(
      Slider::selectionInnerColour, "Slider", "selectionInnerColour");
    sheet.applyColour(
      Slider::selectionActiveColour, "Slider", "selectionActiveColour");
    sheet.applyFloat(Slider::selectionWidth, "Slider", "selectionWidth");
    sheet.applyFloat(Slider::selectionSize, "Slider", "selectionSize");
    sheet.applyFloat(
      Slider::selectionActivePadding, "Slider", "selectionActivePadding");
  }
  //============================================================================
private:
  StyleSheet sheet;
};
//==============================================================================
} // namespace theme
} // namespace dmt