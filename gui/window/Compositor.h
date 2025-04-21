//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ███████ ██    ██   ███     ████
 * ██   ██ ██ ██  ██  ██ ██         ██    ██   ██ ██    ██  ██ ██     ██
 * ██████  ██ ██      ██ ███████    ██    ██   ██  ██████  ██   ██    ██
 *
 * Copyright (C) 2024 Dimethoxy Audio (https://dimethoxy.com)
 *
 * This file is part of the Dimethoxy Library, a collection of essential
 * classes used across various Dimethoxy projects.
 * These files are primarily designed for internal use within our repositories.
 *
 * License:
 * This code is licensed under the GPLv3 license. You are permitted to use and
 * modify this code under the terms of this license.
 * You must adhere GPLv3 license for any project using this code or parts of it.
 * Your are not allowed to use this code in any closed-source project.
 *
 * Description:
 * The `Compositor` acts as the top-level component of the GUI, managing
 * functionality like switching views, showing popups or managing layout.
 *
 * Authors:
 * Lunix-420 (Primary Author)
 */
//==============================================================================

#pragma once

//==============================================================================

#include "dmt/version/Info.h"
#include "gui/panel/AbstractPanel.h"
#include "gui/panel/SettingsPanel.h"
#include "gui/widget/BorderButton.h"
#include "gui/window/Header.h"
#include "gui/window/Popover.h"
#include <JuceHeader.h>

//==============================================================================

namespace dmt {
namespace gui {
namespace window {

//==============================================================================
/**
 * @brief Manages the layout and behavior of the main application window.
 *
 * The `Compositor` class handles the dynamic resizing, visibility toggling,
 * and interaction logic for the header, panels, and popovers. It ensures
 * that the UI remains responsive and intuitive while adhering to real-time
 * performance constraints.
 */
class Compositor
  : public juce::Component
  , public juce::Timer
{
  //============================================================================
  // Feature flag for update notification
#ifdef DMT_DISABLE_UPDATE_NOTIFICATION
  static constexpr bool updateNotificationEnabled = false;
#else
  static constexpr bool updateNotificationEnabled = true;
#endif
  //============================================================================
  // Aliases for convenience
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using SettingsPanel = dmt::gui::panel::SettingsPanel;
  using Header = dmt::gui::window::Header;
  using BorderButton = dmt::gui::widget::BorderButton;
  using Popover = dmt::gui::window::Popover;
  using TooltipWindow = juce::TooltipWindow;

  //============================================================================
  // Window size
  const float& size = dmt::Settings::Window::size;
  const int rawHeaderHeight = dmt::Settings::Header::height;

  // Header
  const int& rawBorderButtonHeight = Settings::Header::borderButtonHeight;

public:
  //============================================================================
  /**
   * @brief Constructs a `Compositor` instance.
   *
   * @param _titleText The title text to display in the header.
   * @param _mainPanel The main panel to manage within the window.
   * @param _apvts The audio processor value tree state for parameter
   * management.
   */
  Compositor(juce::String _titleText,
             AbstractPanel& _mainPanel,
             AudioProcessorValueTreeState& _apvts) noexcept
    : mainPanel(_mainPanel)
    , settingsPanel()
    , header(_titleText, _apvts)
    , borderButton()
    , tooltipWindow(this, 700)
  {
    // Header
    addAndMakeVisible(header);
    header.getSettingsButton().onClick = [this] { showSettings(); };
    header.getSettingsExitButton().onClick = [this] { closeSettings(); };
    header.getHideHeaderButton().onClick = [this] { hideHeader(); };

    // BorderButton
    addChildComponent(borderButton);
    borderButton.setButtonCallback([this]() { showHeader(); });

    // Popover
    addChildComponent(popover);

    // Panels
    addAndMakeVisible(mainPanel);
    addChildComponent(settingsPanel);

    // Start the timer to check if update is found
    if (updateNotificationEnabled)
      startTimer(1000); // Commented out until Popover is done

    // Tooltips
    addAndMakeVisible(tooltipWindow);
  }

  //============================================================================
  /** @brief Destructor for `Compositor`. */
  ~Compositor() noexcept override = default;

  //============================================================================
  /** @brief Paints the component. */
  void paint(juce::Graphics& /*_g*/) noexcept override {}

  //============================================================================
  /**
   * @brief Handles resizing of the component and its children.
   *
   * Dynamically adjusts the bounds of the header, panels, and buttons
   * based on the visibility state of the header.
   */
  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();

    // Popover
    popover.setBounds(bounds);

    if (header.isVisible()) {
      const auto headerHeight = rawHeaderHeight * size;
      const auto headerBounds = juce::Rectangle(bounds).removeFromTop(
        static_cast<int>(headerHeight * 2.0f));
      header.setBounds(headerBounds);

      const auto contentHeight = bounds.getHeight() - headerHeight;
      const auto contentBounds = juce::Rectangle(bounds).removeFromBottom(
        static_cast<int>(contentHeight));
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
      borderButton.setBounds(juce::Rectangle<int>(bounds).removeFromTop(
        static_cast<int>(borderButtonHeight)));
      borderButton.setVisible(true);
      borderButton.setAlwaysOnTop(true);
    }
  }

  //==============================================================================
  /** @brief Shows the settings panel and hides the main panel. */
  void showSettings() noexcept
  {
    mainPanel.setVisible(false);
    settingsPanel.setVisible(true);
    header.getSettingsButton().setVisible(false);
    header.getSettingsExitButton().setVisible(true);
    repaint();
  }

  //==============================================================================
  /** @brief Closes the settings panel and shows the main panel. */
  void closeSettings() noexcept
  {
    mainPanel.setVisible(true);
    settingsPanel.setVisible(false);
    header.getSettingsButton().setVisible(true);
    header.getSettingsExitButton().setVisible(false);
    repaint();
  }

  //==============================================================================
  /** @brief Hides the header and adjusts the layout accordingly. */
  void hideHeader() noexcept
  {
    header.setVisible(false);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(false);
    }
    borderButton.setOpacityToMax();
    popover.hideMessage();
  }

  //==============================================================================
  /** @brief Shows the header and adjusts the layout accordingly. */
  void showHeader() noexcept
  {
    header.setVisible(true);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(true);
    }
  }

  //==============================================================================
  /**
   * @brief Timer callback to check for updates.
   *
   * If an update is available, it shows the update popover and button.
   */
  void timerCallback() override
  {
    if (!updateNotificationEnabled)
      return;

    if (dmt::version::Info::isLatest != nullptr &&
        !(*dmt::version::Info::isLatest)) {
      showUpdatePopover();
      showUpdateButton();
      stopTimer();
    }
  }

  //==============================================================================
  /** @brief Displays the update popover with a message. */
  void showUpdatePopover() noexcept
  {
    if (!updateNotificationEnabled)
      return;

    if (!dmt::version::Info::wasPopoverShown) {
      const auto& updateButton = header.getUpdateButton();
      const int x = updateButton.getBounds().getCentreX();
      const int yOffset = static_cast<int>(-10.0f * size);
      const int y = updateButton.getBounds().getBottom() + yOffset;
      const auto popoverTargetPoint = juce::Point<int>(x, y);
      juce::String title = "Update Available!";
      juce::String message;
      message << "A new update is available! \n"
              << "Click here to download the latest version. ";
      popover.showMessage(popoverTargetPoint, title, message);
      dmt::version::Info::wasPopoverShown = true;
    }
  }

  //==============================================================================
  /** @brief Makes the update button visible. */
  void showUpdateButton() noexcept
  {
    if (!updateNotificationEnabled)
      return;

    header.getUpdateButton().setVisible(true);
  }

  //==============================================================================
  /**
   * @brief Sets a callback for header visibility changes.
   *
   * @param callback A function to call when the header visibility changes.
   */
  void setHeaderVisibilityCallback(std::function<void(bool)> callback)
  {
    headerVisibilityCallback = std::move(callback);
  }

  //==============================================================================
  /**
   * @brief Checks if the header is currently visible.
   *
   * @return `true` if the header is visible, `false` otherwise.
   */
  [[nodiscard]] bool isHeaderVisible() const noexcept
  {
    return header.isVisible();
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  AbstractPanel& mainPanel;
  SettingsPanel settingsPanel;
  Header header;
  BorderButton borderButton;
  TooltipWindow tooltipWindow;

  //==============================================================================
  // Other members
  std::function<void(bool)> headerVisibilityCallback;
  Popover popover;
};

} // namespace window
} // namespace gui
} // namespace dmt