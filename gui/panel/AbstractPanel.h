//==============================================================================
#pragma once
//==============================================================================
#include "gui/widget/Label.h"
#include "gui/widget/Shadow.h"
#include "gui/widget/TriangleButton.h"
#include "utility/Fonts.h"
#include "utility/Settings.h"
#include <JuceHeader.h>
//==============================================================================
namespace dmt {
namespace gui {
namespace panel {
//==============================================================================
class AbstractPanel
  : public juce::Component
  , public juce::Button::Listener
{
public:
  using Grid = std::vector<std::vector<juce::Point<float>>>;
  using TriangleButton = dmt::gui::widget::TriangleButton;
  using Shadow = dmt::gui::widget::Shadow;
  using Label = dmt::gui::widget::Label;
  using Fonts = dmt::utility::Fonts;

  // Settings
  using LibrarySettings = dmt::Settings;
  using Settings = LibrarySettings::Panel;
  using Carousel = LibrarySettings::Carousel;
  // Layout
  const float& size = LibrarySettings::Window::size;
  const float& margin = LibrarySettings::Window::margin;
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

  AbstractPanel(const juce::String name, const bool displayName = true) noexcept
    : layout({ 1, 1 })
    , rawGridOffsetY(40 * static_cast<int>(displayName))
    , name(name)
    , titleLabel(name, fonts.bold, fontSize, juce::Colours::white)
    , nextCallback([]() {})
    , prevCallback([]() {})
    , nextButton(dmt::gui::widget::TriangleButton::Right)
    , prevButton(dmt::gui::widget::TriangleButton::Left)
    , outerShadow(drawOuterShadow, outerShadowColour, outerShadowRadius, false)
    , innerShadow(drawInnerShadow, outerShadowColour, outerShadowRadius, true)
  {
    if (displayName) {
      addAndMakeVisible(titleLabel);
    }
    setLayout(layout);
    addAndMakeVisible(outerShadow);
    addAndMakeVisible(innerShadow);
    setOpaque(true);
  }

  void paint(juce::Graphics& g) noexcept override
  {
    TRACER("AbstractPanel:paint");
    // Precalculation
    const auto bounds = this->getLocalBounds().toFloat();
    const auto outerBounds = bounds.reduced(margin * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = cornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    // draw debug bounds
    g.setColour(juce::Colours::aqua);
    if (debugBounds)
      g.drawRect(bounds, 1.0f);

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

    // draw debug line grid
    if (debugGrid) {
      g.setColour(juce::Colours::red);
      for (size_t col = 0; col < grid.size(); col++) {
        const auto firstPoint = getGridPoint(bounds.toNearestInt(), col, 0);
        const auto endPoint =
          juce::Point<int>(firstPoint.getX(), (int)bounds.getHeight() - 1);
        const auto line =
          juce::Line<float>(firstPoint.toFloat(), endPoint.toFloat());
        g.drawLine(line, 1.0f);
      }
      for (size_t row = 0; row < grid[0].size(); row++) {
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
    TRACER("AbstractPanel:resized");
    const auto bounds = getLocalBounds();
    const auto outerBounds = bounds.reduced(margin * size);
    const auto innerBounds = outerBounds.reduced(borderStrength * size);
    const float outerCornerSize = cornerSize * size;
    const float innerCornerSize = std::clamp(
      outerCornerSize - (borderStrength * size * 0.5f), 0.0f, outerCornerSize);

    juce::Path outerShadowPath;
    outerShadowPath.addRoundedRectangle(outerBounds, outerCornerSize);
    outerShadow.setPath(outerShadowPath);
    outerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    outerShadow.toBack();

    juce::Path innerShadowPath;
    innerShadowPath.addRoundedRectangle(innerBounds, innerCornerSize);
    innerShadow.setPath(innerShadowPath);
    innerShadow.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    innerShadow.toBack();

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

    titleLabel.setBounds(bounds.reduced(padding + marginSize));

    extendResize();
  }

  virtual void extendResize() noexcept {}
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
    TRACER("AbstractPanel:setLayout");
    const int cols = layoutToUse.cols;
    const int rows = layoutToUse.rows;
    const float colSpacing = 1.0f / (float)(cols + 1);
    const float rowSpacing = 1.0f / (float)(rows + 1);
    Grid newGrid(cols + 2, std::vector<juce::Point<float>>(rows + 2));

    for (size_t col = 0; col <= (layoutToUse.cols + 1); col++) {
      for (size_t row = 0; row <= (layoutToUse.rows + 1); row++) {
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
    TRACER("AbstractPanel:getGridPoint");
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
  Fonts fonts;
  Layout layout;
  Grid grid;
  int rawGridOffsetY;
  const juce::String name;
  Label titleLabel;
  std::function<void()> nextCallback;
  std::function<void()> prevCallback;
  TriangleButton nextButton;
  TriangleButton prevButton;
  Shadow outerShadow;
  Shadow innerShadow;
  //============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbstractPanel)
};
} // namespace panel
} // namespace gui
} // namespace dmt
