#pragma once

#include "gui/panel/AbstractPanel.h"
#include "gui/panel/SettingsPanel.h"
#include "gui/widget/BorderButton.h"
#include "gui/window/Header.h"
#include "gui/window/Popover.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {

class Compositor
  : public juce::Component
  , public juce::Timer
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using SettingsPanel = dmt::gui::panel::SettingsPanel;
  using Header = dmt::gui::window::Header;
  using BorderButton = dmt::gui::widget::BorderButton;
  using Popover = dmt::gui::window::Popover;

  // Window size
  const float& size = dmt::Settings::Window::size;
  const int rawHeaderHeight = dmt::Settings::Header::height;

  // Header
  const int& rawBorderButtonHeight = Settings::Header::borderButtonHeight;

public:
  Compositor(juce::String _titleText,
             AbstractPanel& _mainPanel,
             AudioProcessorValueTreeState& _apvts)
    : mainPanel(_mainPanel)
    , settingsPanel()
    , header(_titleText, _apvts)
    , borderButton()
  {
    // Header
    addAndMakeVisible(header);
    header.getSettingsButton().onClick = [this] { showSettings(); };
    header.getSettingsExitButton().onClick = [this] { closeSettings(); };
    header.getHideHeaderButton().onClick = [this] { hideHeader(); };

    // BorderButton
    addAndMakeVisible(borderButton);
    borderButton.setVisible(false);
    borderButton.setButtonCallback([this]() { showHeader(); });

    // Popover
    addAndMakeVisible(popover);

    // Panels
    addAndMakeVisible(mainPanel);
    addAndMakeVisible(settingsPanel);
    settingsPanel.setVisible(false);

    // Start the timer to check if update is found
    startTimer(1000); // Check every second
  }

  ~Compositor() noexcept override {}

  void paint(juce::Graphics& /*_g*/) noexcept override {}

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();

    if (header.isVisible()) {
      const auto headerHeight = rawHeaderHeight * size;
      const auto headerBounds =
        juce::Rectangle(bounds).removeFromTop(headerHeight * 2.0f);
      header.setBounds(headerBounds);

      const auto contentHeight = bounds.getHeight() - headerHeight;
      const auto contentBounds =
        juce::Rectangle(bounds).removeFromBottom(contentHeight);
      mainPanel.setBounds(contentBounds);
      settingsPanel.setBounds(contentBounds);

      borderButton.setVisible(
        false); // Hide the BorderButton when the header is visible
    } else {
      // If the header is hidden, the main panel takes the full bounds
      mainPanel.setBounds(bounds);
      settingsPanel.setBounds(bounds);

      // Show the BorderButton at the top with half the height of the header
      const auto borderButtonHeight = rawBorderButtonHeight * size;
      borderButton.setBounds(
        juce::Rectangle<int>(bounds).removeFromTop(borderButtonHeight));
      borderButton.setVisible(true);
    }

    // Popover
    popover.setBounds(bounds);
  }

  void showSettings() noexcept
  {
    mainPanel.setVisible(false);
    settingsPanel.setVisible(true);
    header.getSettingsButton().setVisible(false);
    header.getSettingsExitButton().setVisible(true);
    repaint();
  }

  void closeSettings() noexcept
  {
    mainPanel.setVisible(true);
    settingsPanel.setVisible(false);
    header.getSettingsButton().setVisible(true);
    header.getSettingsExitButton().setVisible(false);
    repaint();
  }

  void hideHeader() noexcept
  {
    header.setVisible(false);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(false);
    }
    borderButton.setOpacityToMax();
  }

  void showHeader() noexcept
  {
    header.setVisible(true);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(true);
    }
  }

  void timerCallback() override
  {
    // Check for updates and show the popover if needed
    if (true) // TODO: Check for updates
    {
      showUpdateButton();
      stopTimer(); // Stop the timer after showing the popover
    }
  }

  void showUpdateButton() noexcept
  {
    if (true) // TODO: Check if we already showed the popover before
    {
      const auto& updateButton = header.getUpdateButton();
      const int x = updateButton.getBounds().getCentreX();
      const int y = updateButton.getBounds().getBottom();
      const auto popoverTagetPoint = juce::Point<int>(x, y);
      popover.showMessage(popoverTagetPoint);
    }
  }

  void setHeaderVisibilityCallback(std::function<void(bool)> callback)
  {
    headerVisibilityCallback = std::move(callback);
  }

  bool isHeaderVisible() const noexcept { return header.isVisible(); }

private:
  std::function<void(bool)> headerVisibilityCallback;
  Popover popover;
  AbstractPanel& mainPanel;
  SettingsPanel settingsPanel;
  Header header;
  BorderButton borderButton; // Add the BorderButton as a member
};
} // namespace window
} // namespace gui
} // namespace dmt