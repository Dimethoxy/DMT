//==============================================================================

#pragma once

#include "gui/widget/Shadow.h"
#include "utility/RepaintTimer.h"
#include "utility/Settings.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
class DisfluxDisplayComponent
  : public juce::Component
  , public dmt::utility::RepaintTimer
{
  using Shadow = dmt::gui::widget::Shadow;

  // General
  using Settings = dmt::Settings::OscillatorDisplay;
  const int& resolution = Settings::resolution;
  // Shadows
  const bool& drawOuterShadow = Settings::drawOuterShadow;
  const bool& drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour& outerShadowColour = Settings::outerShadowColour;
  const juce::Colour& innerShadowColour = Settings::innerShadowColour;
  const float& outerShadowRadius = Settings::outerShadowRadius;
  const float& innerShadowRadius = Settings::innerShadowRadius;
  const float& size = dmt::Settings::Window::size;
  const float& rawPadding = dmt::Settings::Panel::padding;

public:
  //============================================================================
  DisfluxDisplayComponent(/*juce::AudioProcessorValueTreeState& apvts*/)
    : // apvts(apvts),
    outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true)
  {
    this->startRepaintTimer();
  }
  //============================================================================
  void paint(juce::Graphics& g) override
  {
    const auto bounds = this->getLocalBounds().toFloat();

    const auto padding = rawPadding * size;
    const auto innerBounds = bounds.reduced(padding);

    if (dmt::Settings::debugBounds) {
      g.setColour(juce::Colours::orange);
      g.drawRect(bounds, 1.0f);
      g.setColour(juce::Colours::magenta);
      g.drawRect(innerBounds, 1.0f);
    }
  }
  //============================================================================
  void resized() override { TRACER("DisfluxDisplayComponent::resized"); }
  //============================================================================
  void repaintTimerCallback() noexcept override { this->repaint(); }

private:
  Shadow outerShadow =
    Shadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false);
  Shadow innerShadow =
    Shadow(drawInnerShadow, innerShadowColour, innerShadowRadius, true);

  // juce::AudioProcessorValueTreeState& apvts;
};
} // namespace components
} // namespace gui
} // namespace dmt