//==============================================================================

#pragma once

#include "../../utility/AppSettings.h"
#include "../../utility/Shadow.h"
#include "../widgets/Label.h"
#include "../widgets/TriangleButton.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
//==============================================================================
class Panel
  : public juce::Component
  , public juce::Button::Listener
{
  using Grid = std::vector<std::vector<juce::Point<float>>>;
  // Settings
  using Settings = dmt::AppSettings::Panel;
  using Fonts = dmt::AppSettings::Fonts;
  using Carousel = dmt::AppSettings::Carousel;
  // Global
  const float& size = dmt::AppSettings::size;
  const float& margin = dmt::AppSettings::Layout::margin;
  // General
  const juce::Colour& backgroundColour = Settings::backgroundColour;
  const float& cornerSize = Settings::cornerSize;
  const float& rawPadding = Settings::padding;
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
  // Fonts
  const juce::Colour& fontColor = Settings::fontColor;
  const float& fontSize = Settings::fontSize;

public:
  struct Layout
  {
    int cols;
    int rows;
  };

  Panel(juce::String name) noexcept
    : layout({ 1, 1 })
    , name(name)
    , titleLabel(name, Fonts::bold, fontSize, juce::Colours::white)
    , nextCallback([]() {})
    , prevCallback([]() {})
    , nextButton(dmt::gui::widgets::TriangleButton::Right)
    , prevButton(dmt::gui::widgets::TriangleButton::Left)
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
  {
    addAndMakeVisible(titleLabel);
  }

  void paint(juce::Graphics& g) noexcept override
  {
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(margin);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = cornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // draw debug bounds
    // g.setColour(juce::Colours::aqua);
    // g.drawRect(bounds, 1.0f);

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

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();
    const int buttonWidth = (int)(Carousel::buttonWidth * size);
    const int buttonHeight = (int)(Carousel::buttonHeight * size);
    const int marginSize = (int)(size * margin);
    const int padding = (int)(rawPadding * size);

    auto leftBounds = bounds;
    auto rightBounds = bounds;

    leftBounds.removeFromRight(bounds.getWidth() - buttonWidth);
    leftBounds.setHeight(buttonHeight);
    leftBounds.setCentre(leftBounds.getCentreX(), bounds.getCentreY());
    prevButton.setBounds(leftBounds.reduced(marginSize));

    rightBounds.removeFromLeft(bounds.getWidth() - buttonWidth);
    rightBounds.setHeight(buttonHeight);
    rightBounds.setCentre(rightBounds.getCentreX(), bounds.getCentreY());
    nextButton.setBounds(rightBounds.reduced(marginSize));

    titleLabel.setBounds(bounds.reduced(padding));
  }

  virtual inline const juce::String getName() noexcept { return "Panel"; }

  void setCallbacks(std::function<void()> next, std::function<void()> prev)
  {
    nextCallback = next;
    prevCallback = prev;
    addAndMakeVisible(nextButton);
    addAndMakeVisible(prevButton);
    nextButton.addListener(this);
    prevButton.addListener(this);
  }

  void next() { nextCallback(); }
  void prev() { prevCallback(); }
  void buttonClicked(juce::Button* button) override
  {
    if (button == &nextButton) {
      nextCallback();
    } else if (button == &prevButton) {
      prevCallback();
    }
  }

protected:
  inline const Layout getLayout() noexcept { return layout; }
  void setLayout(const Layout layoutToUse) noexcept
  {
    const int cols = layoutToUse.cols;
    const int rows = layoutToUse.rows;
    const float colSpacing = 1.0f / (float)(cols + 1);
    const float rowSpacing = 1.0f / (float)(rows + 1);
    Grid newGrid(cols + 2, std::vector<juce::Point<float>>(rows + 2));

    for (int col = 0; col <= (layoutToUse.cols + 1); col++) {
      for (int row = 0; row <= (layoutToUse.rows + 1); row++) {
        const float x = (float)col * colSpacing;
        const float y = (float)row * rowSpacing;
        const auto point = juce::Point<float>(x, y);
        newGrid[col][row] = point;
      }
    }
    this->grid = newGrid;
    this->layout = layoutToUse;
  }
  inline const juce::Point<int> getGridPoint(const juce::Rectangle<int> bounds,
                                             const int col,
                                             const int row) noexcept
  {
    auto rawPoint = grid[col][row];
    juce::Point<float> point(rawPoint.getX() * (float)bounds.getWidth(),
                             rawPoint.getY() * (float)bounds.getHeight());
    return point.toInt();
  }

private:
  Layout layout;
  Grid grid;
  const juce::String name;
  dmt::gui::widgets::Label titleLabel;
  std::function<void()> nextCallback;
  std::function<void()> prevCallback;
  dmt::gui::widgets::TriangleButton nextButton;
  dmt::gui::widgets::TriangleButton prevButton;
  dmt::Shadow outerShadow;
  dmt::Shadow innerShadow;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Panel)
};

//==============================================================================
} // namespace gui
} // namespace dmt
