#pragma once

#include "gui/panel/AbstractPanel.h"
#include "gui/panel/SettingsPanel.h"
#include "gui/widget/BorderButton.h" // Include the BorderButton header
#include "gui/window/Header.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {

class Compositor : public juce::Component
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using SettingsPanel = dmt::gui::panel::SettingsPanel;
  using Header = dmt::gui::window::Header;
  using BorderButton = dmt::gui::widget::BorderButton;

  // Window size
  const float& size = dmt::Settings::Window::size;
  const int rawHeaderHeight = dmt::Settings::Header::height;

public:
  Compositor(juce::String _titleText, AbstractPanel& _mainPanel)
    : mainPanel(_mainPanel)
    , settingsPanel()
    , header(_titleText)
    , borderButton()
  {
    addAndMakeVisible(mainPanel);
    addAndMakeVisible(settingsPanel);
    addAndMakeVisible(header);
    addAndMakeVisible(borderButton); // Add the BorderButton to the hierarchy

    settingsPanel.setVisible(false);
    borderButton.setVisible(false); // Initially hidden

    header.getSettingsButton().onClick = [this] { showSettings(); };
    header.getSettingsExitButton().onClick = [this] { closeSettings(); };
    header.getHideHeaderButton().onClick = [this] { hideHeader(); };

    // Set the BorderButton's callback to show the header
    borderButton.setButtonCallback([this]() { showHeader(); });
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
      const auto borderButtonHeight = (rawHeaderHeight * size) / 2.0f;
      borderButton.setBounds(
        juce::Rectangle<int>(bounds).removeFromTop(borderButtonHeight));
      borderButton.setVisible(true);
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
  }

  void showHeader() noexcept
  {
    header.setVisible(true);
    resized();
    if (headerVisibilityCallback) {
      headerVisibilityCallback(true);
    }
  }

  void setHeaderVisibilityCallback(std::function<void(bool)> callback)
  {
    headerVisibilityCallback = std::move(callback);
  }

  bool isHeaderVisible() const noexcept { return header.isVisible(); }

private:
  std::function<void(bool)> headerVisibilityCallback;
  AbstractPanel& mainPanel;
  SettingsPanel settingsPanel;
  Header header;
  BorderButton borderButton; // Add the BorderButton as a member
};
} // namespace window
} // namespace gui
} // namespace dmt