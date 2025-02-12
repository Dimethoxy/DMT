#pragma once

#include "dmt/gui/widget/Label.h"
#include "dmt/utility/Settings.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {
class Header : public juce::Component
{
  // Aliases
  using Label = dmt::gui::widget::Label;
  using Fonts = dmt::utility::Fonts;
  using HeaderSettings = dmt::Settings::Header;

  // General
  const juce::String name = ProjectInfo::projectName;
  const float& size = dmt::Settings::Window::size;
  const float& rawBorderStrength = dmt::Settings::Panel::borderStrength;
  const juce::Colour& backroundColour = HeaderSettings::backroundColour;
  const juce::Colour& borderColor = dmt::Settings::Header::borderColor;

  // Labels
  const juce::Colour& titleFontColour = HeaderSettings::titleFontColour;
  const float& titleFontSize = HeaderSettings::titleFontSize;
  const float& rawTitleOffset = HeaderSettings::titleOffset;

public:
  Header(juce::String titleText)
    : title(juce::String("ProjectLabel"),
            fonts.display,
            titleFontSize,
            juce::Colours::white,
            juce::Justification::centred)
  {
    title.setText(titleText);
    addAndMakeVisible(title);
  };

  ~Header() override {}

  void paint(juce::Graphics& g) override
  {
    // Paint the border
    auto bounds = getLocalBounds();
    g.setColour(borderColor);
    g.fillRect(bounds);

    // Paint the background
    const auto borderStrength = rawBorderStrength * size;
    bounds.removeFromBottom(borderStrength);
    g.setColour(backroundColour);
    g.fillRect(bounds);
  }

  void resized() override
  {
    auto bounds = getLocalBounds();
    const auto titleOffset = rawTitleOffset * size;
    const auto titleBounds = bounds.withY(titleOffset);
    title.setBounds(titleBounds);
  }

private:
  Label title;
  Fonts fonts;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
} // namespace component
} // namespace gui
} // namespace dmt