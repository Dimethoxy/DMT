//==============================================================================
#pragma once
//==============================================================================
#include "../../utility/LibrarySettings.h"
#include "../widget/Label.h"
#include "../widget/Shadow.h"
#include "../widget/TriangleButton.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
//==============================================================================
class Panel
  : public juce::Component
  , public juce::Button::Listener
{
public:
  using Grid = std::vector<std::vector<juce::Point<float>>>;
  // Settings
  using LibrarySettings = dmt::LibrarySettings;
  using Settings = LibrarySettings::Panel;
  using Fonts = LibrarySettings::Fonts;
  using Carousel = LibrarySettings::Carousel;
  // Layout
  const float& size = LibrarySettings::Layout::size;
  const float& margin = LibrarySettings::Layout::margin;
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
  // Debug
  const bool& debugBounds = LibrarySettings::debugBounds;
  const bool& debugGrid = LibrarySettings::debugGrid;

  struct Layout
  {
    int cols;
    int rows;
  };

  Panel(const juce::String name, const bool displayName = true) noexcept
    : layout({ 1, 1 })
    , rawGridOffsetY(40 * static_cast<int>(displayName))
    , name(name)
    , titleLabel(name, Fonts::bold, fontSize, juce::Colours::white)
    , nextCallback([]() {})
    , prevCallback([]() {})
    , nextButton(dmt::gui::widgets::TriangleButton::Right)
    , prevButton(dmt::gui::widgets::TriangleButton::Left)
    , outerShadow(outerShadowColour, outerShadowRadius)
    , innerShadow(innerShadowColour, innerShadowRadius)
  {
    if (displayName) {
      addAndMakeVisible(titleLabel);
    }
    setLayout(layout);
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
    g.setColour(juce::Colours::aqua);
    if (debugBounds)
      g.drawRect(bounds, 1.0f);

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

    // draw debug line grid
    if (debugGrid) {
      g.setColour(juce::Colours::red);
      for (int col = 0; col < grid.size(); col++) {
        const auto firstPoint = getGridPoint(bounds.toNearestInt(), col, 0);
        const auto endPoint =
          juce::Point<int>(firstPoint.getX(), (int)bounds.getHeight() - 1);
        const auto line =
          juce::Line<float>(firstPoint.toFloat(), endPoint.toFloat());
        g.drawLine(line, 1.0f);
      }
      for (int row = 0; row < grid[0].size(); row++) {
        const auto firstPoint = getGridPoint(bounds.toNearestInt(), 0, row);
        const auto endPoint =
          juce::Point<int>((int)bounds.getWidth() - 1, firstPoint.getY());
        const auto line =
          juce::Line<float>(firstPoint.toFloat(), endPoint.toFloat());
        g.drawLine(line, 1.0f);
      }
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
    // assert if col and row are out of bounds
    jassert(col >= 0 && col < grid.size());
    jassert(row >= 0 && row < grid[col].size());

    auto rawPoint = grid[col][row];

    const auto x = rawPoint.getX() * (float)bounds.getWidth();

    const float gridOffsetY = (float)rawGridOffsetY * size;
    const float offsetBoundsHeight = bounds.getHeight() - gridOffsetY;
    const auto y = rawPoint.getY() * offsetBoundsHeight + gridOffsetY;

    juce::Point<float> point(x, y);
    return point.toInt();
  }

  void setRawGridOffset(const int offset) noexcept { rawGridOffsetY = offset; }
  const inline int getRawGridOffset() const noexcept { return rawGridOffsetY; }

private:
  Layout layout;
  Grid grid;
  int rawGridOffsetY;
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
