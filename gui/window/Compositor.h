#pragma once

#include "dmt/version/Info.h"
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
  using TooltipWindow = juce::TooltipWindow;

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
    startTimer(1000); // Commented out until Popover is done

    // Tooltips
    addAndMakeVisible(tooltipWindow);
  }

  ~Compositor() noexcept override {}

  void paint(juce::Graphics& /*_g*/) noexcept override {}

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();

    // Popover
    popover.setBounds(bounds);

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
      borderButton.setAlwaysOnTop(true);
    }
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
    popover.hideMessage();
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
    if (dmt::version::Info::isLatest != nullptr &&
        !(*dmt::version::Info::isLatest)) {
      showUpdateButton();
      stopTimer();
    }
  }

  void showUpdateButton() noexcept
  {
    if (!dmt::version::Info::wasPopoverShown) {
      const auto& updateButton = header.getUpdateButton();
      const int x = updateButton.getBounds().getCentreX();
      const int yOffset = -10.0f * size;
      const int y = updateButton.getBounds().getBottom() + yOffset;
      const auto popoverTagetPoint = juce::Point<int>(x, y);
      String title = "Update Available!";
      String message;
      message << "A new update is available! \n"
              << "Click here to download the latest version. ";
      popover.showMessage(popoverTagetPoint, title, message);
      header.getUpdateButton().setVisible(true);
      dmt::version::Info::wasPopoverShown = true;
    }
  }

  void setHeaderVisibilityCallback(std::function<void(bool)> callback)
  {
    headerVisibilityCallback = std::move(callback);
  }

  bool isHeaderVisible() const noexcept { return header.isVisible(); }

private:
  TooltipWindow tooltipWindow;
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