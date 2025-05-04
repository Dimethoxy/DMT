//==============================================================================
/*
 * ██████  ██ ███    ███ ███████ ████████ ██   ██  ██████  ██   ██ ██    ██
 * ██   ██ ██ ████  ████ ██         ██    ██   ██ ██    ██  ██ ██   ██  ██
 * ██   ██ ██ ██ ████ ██ █████      ██    ██   ██ ██    ██   ███     ████
 * ██   ██ ██ ██  ██  ██ ██         ██    ██ ██    ██   ██ ██     ██
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

#include "configuration/Properties.h"
#include "dmt/utility/Scaleable.h"
#include "dmt/version/Info.h"
#include "gui/panel/AbstractPanel.h"
#include "gui/panel/SettingsPanel.h"
#include "gui/widget/BorderButton.h"
#include "gui/window/Alerts.h"
#include "gui/window/Header.h"
#include "gui/window/Popover.h"
#include "gui/window/Tooltip.h"
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
 *
 * @note This class is designed to be used as a top-level component within a
 *       DMT-based application.
 *
 * @warning This class hides a lot of dark magic and is not intended to be
 *          subclassed or modified directly. Keep your hands off unless you
 *          know what you're doing.
 */
class Compositor
  : public juce::Component
  , public juce::Timer
  , public dmt::gui::component::ValueEditor::Listener
  , public dmt::Scaleable<Compositor>
  , public juce::ComponentListener
{
  //============================================================================
  // Aliases for convenience
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using SettingsPanel = dmt::gui::panel::SettingsPanel;
  using Header = dmt::gui::window::Header;
  using BorderButton = dmt::gui::widget::BorderButton;
  using Popover = dmt::gui::window::Popover;
  using TooltipWindow = juce::TooltipWindow;
  using Properties = dmt::configuration::Properties;
  using String = juce::String;
  using Tooltip = dmt::gui::window::Tooltip;
  using Alerts = dmt::gui::window::Alerts;

  //============================================================================
  // Header
  const int& rawBorderButtonHeight = Settings::Header::borderButtonHeight;
  const int& rawHeaderHeight = dmt::Settings::Header::height;

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
  Compositor(String _titleText,
             AbstractPanel& _mainPanel,
             AudioProcessorValueTreeState& _apvts,
             Properties& _properties,
             const float& _sizeFactor) noexcept
    : juce::Component("Compositor")
    , mainPanel(_mainPanel)
    , properties(_properties)
    , header(_titleText, _apvts)
    , settingsPanel()
    , borderButton()
    , sizeFactor(_sizeFactor)
  {
    TRACER("Compositor::Compositor");
    // Header
    addAndMakeVisible(header);
    header.getSettingsButton().onClick = [this] { settingsCallback(); };
    header.getSettingsExitButton().onClick = [this] { settingExitCallback(); };
    header.getHideHeaderButton().onClick = [this] { hideHeaderCallback(); };
    header.getUpdateButton().onClick = [this] { updateCallback(); };
    header.getSaveButton().onClick = [this] { saveSettingsCallback(); };
    header.getResetButton().onClick = [this] { resetSettingsCallback(); };

    // BorderButton
    addChildComponent(borderButton);
    borderButton.setButtonCallback([this]() { showHeaderCallback(); });

    // Popover
    addChildComponent(popover);

    // Tooltip
    addAndMakeVisible(tooltip);

    // Alerts
    addAndMakeVisible(alerts);

    // Panels
    addAndMakeVisible(mainPanel);
    addChildComponent(settingsPanel);

    // Start the timer to check if update is found
    startTimer(1000);

    // Start listening for hierarchy changes
    addListenerRecursively(this);
  }

  //============================================================================
  /** @brief Destructor for `Compositor`. */
  ~Compositor() noexcept override { removeListenerRecursively(this); }

  //============================================================================
  /** @brief Paints the component. */
  void paint(juce::Graphics& /*_g*/) noexcept override
  {
    TRACER("Compositor::paint");
  }

  //============================================================================
  /**
   * @brief Handles resizing of the component and its children.
   *
   * @details This function is called whenever the component is resized.
   *          It adjusts the layout of header and panels.
   */
  void resized() noexcept override
  {
    TRACER("Compositor::resized");
    const auto bounds = getLocalBounds();

    // Alerts
    alerts.setBounds(bounds);
    alerts.setAlwaysOnTop(true);

    // Popover
    popover.setBounds(bounds);
    popover.setAlwaysOnTop(true);

    // Tooltip
    tooltip.setBounds(bounds);
    tooltip.setAlwaysOnTop(true);

    // Main panel
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
  /**
   * @brief Callback function for the settings button.
   *
   * @details This function is called when the settings button is clicked.
   *          It hides the main panel and shows the settings panel.
   *          The settings button is hidden and the exit button is shown.
   *
   * @note This function is only enabled if the settings panel is not
   *       currently visible.
   */
  void settingsCallback() noexcept
  {
    TRACER("Compositor::settingsCallback");
    if (settingsPanel.isVisible())
      return;

    mainPanel.setVisible(false);
    settingsPanel.setVisible(true);

    // Hide all header buttons except settingsExitButton
    header.getSettingsButton().setVisible(false);
    header.getHideHeaderButton().setVisible(false);
    header.getUpdateButton().setVisible(false);
    header.getSettingsExitButton().setVisible(true);

    // Show settings-only buttons
    header.getResetButton().setVisible(true);
    header.getSaveButton().setVisible(true);

    popover.hideMessage();
    repaint();
  }

  //==============================================================================
  /**
   * @brief Callback function for the close settings button.
   *
   * @details This function is called when the close settings button is clicked.
   *          It hides the settings panel and shows the main panel.
   *
   * @note This function is only enabled if the settings panel is currently
   *       visible.
   */
  void settingExitCallback() noexcept
  {
    TRACER("Compositor::settingExitCallback");
    if (!settingsPanel.isVisible())
      return;

    mainPanel.setVisible(true);
    settingsPanel.setVisible(false);

    // Rerun logic for which buttons to show
    header.getSettingsButton().setVisible(true);
    header.getSettingsExitButton().setVisible(false);

    // Hide settings-only buttons
    header.getResetButton().setVisible(false);
    header.getSaveButton().setVisible(false);

    // Show/hide update button depending on update state
    if (!DMT_DISABLE_UPDATE_NOTIFICATION &&
        dmt::version::Info::isLatest != nullptr &&
        !(*dmt::version::Info::isLatest)) {
      header.getUpdateButton().setVisible(true);
    } else {
      header.getUpdateButton().setVisible(false);
    }

    header.getHideHeaderButton().setVisible(true);
    repaint();
  }

  //==============================================================================
  /**
   * @brief Callback function for the update button.
   *
   * @details This function is called when the update button is clicked.
   *          It launches the download link in the default web browser.
   *
   * @note This function is only enabled if the update notification is not
   *       disabled (DMT_DISABLE_UPDATE_NOTIFICATION is not set).
   *       If the download link is not available, this function does nothing.
   */
  void updateCallback() noexcept
  {
    TRACER("Compositor::updateCallback");
    // Check if update system is disabled
    if (DMT_DISABLE_UPDATE_NOTIFICATION)
      return;

    // Hide the popover
    popover.hideMessage();

    // Check if the download link is available
    if (dmt::version::Info::downloadLink != nullptr) {
      std::cout << "Opening download link: "
                << *dmt::version::Info::downloadLink << std::endl;
      juce::URL(*dmt::version::Info::downloadLink).launchInDefaultBrowser();
    }
  }

  //==============================================================================
  /**
   * @brief Hides the header and adjusts the layout accordingly.
   *
   * @details This function is called when the hide header button is clicked.
   *          It hides the header and shows the border button.
   *
   * @note This function is only enabled if the header is currently visible.
   */
  void hideHeaderCallback() noexcept
  {
    TRACER("Compositor::hideHeaderCallback");
    if (!header.isVisible())
      return;

    header.setVisible(false);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(false);
    }
    borderButton.setOpacityToMax();
    popover.hideMessage();
  }

  //==============================================================================
  /**
   * @brief Shows the header and adjusts the layout accordingly.
   *
   * @details This function is called when the border button is clicked.
   *          It shows the header and hides the border button.
   *
   * @note This function is only enabled if the header is currently hidden.
   */
  void showHeaderCallback() noexcept
  {
    TRACER("Compositor::showHeaderCallback");
    if (header.isVisible())
      return;

    header.setVisible(true);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(true);
    }
  }

  //==============================================================================
  /**
   * @brief Saves the settings to the properties.
   *
   * @details This function is called when the save settings button is clicked.
   *          It saves the current settings to the properties.
   *
   * @note This function is only enabled if the settings panel is currently
   *       visible.
   */
  void saveSettingsCallback() noexcept
  {
    TRACER("Compositor::saveSettingsCallback");
    properties.saveCurrentSettings();
    alerts.pushAlert("Settings saved successfully!",
                     "Your settings have been saved.",
                     Alerts::AlertType::Success);
  }

  //==============================================================================
  /**
   * @brief Resets the settings to the default values.
   *
   * @details This function is called when the reset settings button is clicked.
   *          It resets the current settings to the default values.
   *
   * @note This function is only enabled if the settings panel is currently
   *       visible.
   */
  void resetSettingsCallback() noexcept
  {
    TRACER("Compositor::resetSettingsCallback");
    properties.resetToFallback();

    // Gotta call this first to recalculate the global size
    const auto& parent = getParentComponent();
    if (parent != nullptr) {
      parent->resized();
    }

    // We need this to clear all cached images
    resizedRecursively(this);

    // Now trigger the actual repaint
    getTopLevelComponent()->repaint();

    alerts.pushAlert("Settings have been reset!",
                     "Save to keep the default values permanently.",
                     Alerts::AlertType::Info);
  }

  //==============================================================================
  /**
   * @brief Timer callback to check for updates.
   *
   * @details This function is called periodically to check if an update
   *          is available. If an update is found, it shows the update popover
   *          and the update button.
   *
   * @note This function is only enabled if the update notification is not
   *       disabled (DMT_DISABLE_UPDATE_NOTIFICATION is not set).
   */
  void timerCallback() override
  {
    TRACER("Compositor::timerCallback");
    if (DMT_DISABLE_UPDATE_NOTIFICATION) {
      stopTimer();
      return;
    }

    if (dmt::version::Info::isLatest != nullptr &&
        !(*dmt::version::Info::isLatest)) {
      showUpdatePopover();
      showUpdateButton();
      stopTimer();
    }
  }

  //==============================================================================
  /**
   * @brief Callback function for value editor changes.
   *
   * @details This function is called when the value editor changes.
   *          It triggers a resize event for all child components.
   */
  void valueEditorListenerCallback() override
  {
    TRACER("Compositor::valueEditorListenerCallback");
    resizedRecursively(this);
  }

  //==============================================================================
  /**
   * @brief Shows the update popover.
   *
   * @details This function is called when an update is available.
   *          It displays a popover message informing the user about the update.
   *          We track if the popover was shown to avoid showing it multiple
   * times if the user closes the editor and reopens it.
   *
   * @note This function is only enabled if the update notification is not
   *       disabled (DMT_DISABLE_UPDATE_NOTIFICATION is not set).
   */
  void showUpdatePopover() noexcept
  {
    TRACER("Compositor::showUpdatePopover");
    if (DMT_DISABLE_UPDATE_NOTIFICATION)
      return;

    if (Settings::displayUpdateNotifications == false)
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
  /**
   * @brief Shows the update button in the header.
   *
   * @details This function is called when an update is available.
   *          It makes the update button visible in the header.
   *
   * @note This function is only enabled if the update notification is not
   *       disabled (DMT_DISABLE_UPDATE_NOTIFICATION is not set).
   */
  void showUpdateButton() noexcept
  {
    TRACER("Compositor::showUpdateButton");
    if (DMT_DISABLE_UPDATE_NOTIFICATION)
      return;

    header.getUpdateButton().setVisible(true);
  }

  //==============================================================================
  /**
   * @brief Sets a callback for header visibility changes.
   *
   * @details This function allows you to set a callback that will be called
   *          whenever the header visibility changes.
   *          This needs to be set by the parent component to handle the window
   *          resizing.
   *
   * @param callback A function to call when the header visibility changes.
   *
   * @example
   * setHeaderVisibilityCallback([this](bool isHeaderVisible) {
   *   const int adjustedHeight = isHeaderVisible ? (baseHeight + headerHeight)
   *                                              : baseHeight;
   *   setConstraints(baseWidth, adjustedHeight);
   *   setSize(baseWidth * size, adjustedHeight * size);
   * });
   */
  void setHeaderVisibilityCallback(std::function<void(bool)> callback) noexcept
  {
    TRACER("Compositor::setHeaderVisibilityCallback");
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
    TRACER("Compositor::isHeaderVisible");
    return header.isVisible();
  }

protected:
  //==============================================================================
  /**
   * @brief Resizes the component and its children recursively.
   *
   * @param component The component to resize.
   *
   * @note This function is used to trigger a resize event for all child
   *       components.
   */
  void resizedRecursively(juce::Component* component)
  {
    TRACER("Compositor::resizedRecursively");
    if (component == nullptr)
      return;

    component->resized();

    for (auto& child : component->getChildren()) {
      if (auto* childComponent = dynamic_cast<juce::Component*>(child)) {
        resizedRecursively(childComponent);
      }
    }
  }

public:
  void setSizeFactor() noexcept
  {
    TRACER("Compositor::setSizeFactor");
    setSizeFactorRecursively(this);
  }

protected:
  void setSizeFactorRecursively(juce::Component* component) noexcept
  {
    TRACER("Compositor::setSizeFactorRecursively");
    if (component == nullptr)
      return;

    if (auto* scaleable = dynamic_cast<dmt::IScaleable*>(component)) {
      scaleable->setSizeFactor(sizeFactor);
    }

    for (auto& child : component->getChildren()) {
      if (auto* childComponent = dynamic_cast<juce::Component*>(child)) {
        setSizeFactorRecursively(childComponent);
      }
    }
  }

  // Listen for children added/removed anywhere in the tree
  void componentChildrenChanged(juce::Component& component) override
  {
    setSizeFactor();
    // Ensure we listen to all new children
    addListenerRecursively(&component);
    setSizeFactor();
  }

  // Recursively add this as a ComponentListener to all descendants
  void addListenerRecursively(juce::Component* c)
  {
    if (!c)
      return;
    c->addComponentListener(this);
    for (auto* child : c->getChildren())
      if (auto* cc = dynamic_cast<juce::Component*>(child))
        addListenerRecursively(cc);
  }

  // Recursively remove this as a ComponentListener from all descendants
  void removeListenerRecursively(juce::Component* c)
  {
    if (!c)
      return;
    c->removeComponentListener(this);
    for (auto* child : c->getChildren())
      if (auto* cc = dynamic_cast<juce::Component*>(child))
        removeListenerRecursively(cc);
  }

private:
  //==============================================================================
  // Members initialized in the initializer list
  AbstractPanel& mainPanel;
  Properties& properties;
  Header header;
  SettingsPanel settingsPanel;
  BorderButton borderButton;

  //==============================================================================
  // Other members
  std::function<void(bool)> headerVisibilityCallback;
  Popover popover;
  Tooltip tooltip;
  Alerts alerts;
  int baseHeight = 0;
  int baseWidth = 0;
  const float& sizeFactor;
};

} // namespace window
} // namespace gui
} // namespace dmt