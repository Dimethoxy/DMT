#pragma once
//==============================================================================
#include "dsp/processor/OscilloscopeProcessor.h"
#include "gui/widget/Curve.h"
#include "utility/RepaintTimer.h"
#include <JuceHeader.h>
#include <gui/widget/Shadow.h>
#include <utility/LibrarySettings.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace component {
//==============================================================================
template<typename SampleType>
class OscilloscopeComponent : public juce::Component
{
  using OscilloscopeProcessor =
    dmt::dsp::processor::OscilloscopeProcessor<SampleType>;
  // Layout
  const float& size = LibrarySettings::Layout::size;
  const float& rawCornerSize = Settings::cornerSize;
  const float& rawPadding = Settings::padding;
  // General
  using LibrarySettings = dmt::LibrarySettings;
  using Settings = LibrarySettings::Oscilloscope;
  const juce::Colour& backgroundColour = Settings::backgroundColour;
  // Border
  const bool& drawBorder = Settings::drawBorder;
  const juce::Colour& borderColour = Settings::borderColour;
  const float& borderStrength = Settings::borderStrength;
  // Shadows
  const bool& drawOuterShadow = Settings::drawOuterShadow;
  const bool& drawInnerShadow = Settings::drawInnerShadow;
  const juce::Colour& outerShadowColour = Settings::outerShadowColour;
  const juce::Colour& innerShadowColour = Settings::innerShadowColour;
  const float& outerShadowRadius = Settings::outerShadowRadius;
  const float& innerShadowRadius = Settings::innerShadowRadius;

public:
  OscilloscopeComponent(OscilloscopeProcessor& processor)
    : processor(processor)
    , curve([this](int numDataPoints) {
      return this->getLeftChannelAmplitudes(numDataPoints);
    })
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
  {
    addAndMakeVisible(curve);
  }

  void paint(juce::Graphics& g) override
  {
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(rawPadding * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // Draw outer shadow
    if (drawOuterShadow) {
      juce::Path outerShadowPath;
      outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
      outerShadow.drawOuterForPath(g, outerShadowPath);
    }

    // Draw background if border is disabled
    if (!drawBorder) {
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
    }

    // Draw background and border if border is enabled
    if (drawBorder) {
      g.setColour(borderColour);
      g.fillRoundedRectangle(outerBounds, outerCornerSize);
      g.setColour(backgroundColour);
      g.fillRoundedRectangle(innerBounds, innerCornerSize);
    }

    // Draw the inner shadow
    if (drawInnerShadow) {
      juce::Path innerShadowPath;
      innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
      innerShadow.drawInnerForPath(g, innerShadowPath);
    }
  }

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto outerBounds = bounds.reduced(rawPadding * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = rawCornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    auto rawCurveBounds = outerBounds;
    if (drawBorder) {
      rawCurveBounds = innerBounds;
    }
    const auto curveBounds =
      rawCurveBounds.withHeight(rawCurveBounds.getHeight() * 0.9f)
        .withCentre(bounds.getCentre());
    curve.setBounds(curveBounds);
  }

  std::vector<SampleType> getLeftChannelAmplitudes(int numDataPoints)
  {
    return processor.getAmplitudes(numDataPoints)[0];
  }

private:
  OscilloscopeProcessor& processor;
  dmt::gui::widget::Curve<SampleType> curve;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;
};
} // namespace component
} // namespace gui
} // namespace dmt