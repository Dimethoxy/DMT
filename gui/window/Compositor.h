#pragma once

#include "gui/panel/AbstractPanel.h"
#include "gui/window/Header.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {

class Compositor : public juce::Component
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using Header = dmt::gui::window::Header;

  // Window size
  const float& size = dmt::Settings::Window::size;
  const int rawHeaderHeight = dmt::Settings::Header::height;

public:
  Compositor(juce::String _titleText, AbstractPanel& _mainPanel)
    : mainPanel(_mainPanel)
    , header(_titleText)
  {
    addAndMakeVisible(header);
    addAndMakeVisible(mainPanel);
    header.getSettingsButton().onClick = [this] { showSettings(); };
    header.getSettingsExitButton().onClick = [this] { closeSettings(); };
  }

  ~Compositor() noexcept override {}

  void paint(juce::Graphics& /*_g*/) noexcept override {}

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();

    const auto headerHeight = rawHeaderHeight * size;
    const auto headerBounds =
      juce::Rectangle(bounds).removeFromTop(headerHeight * 2.0f);
    header.setBounds(headerBounds);

    const auto contentHeight = bounds.getHeight() - headerHeight;
    const auto contentBounds =
      juce::Rectangle(bounds).removeFromBottom(contentHeight);
    mainPanel.setBounds(contentBounds);
  }

  void showSettings() noexcept
  {
    mainPanel.setVisible(false);
    header.getSettingsButton().setVisible(false);
    header.getSettingsExitButton().setVisible(true);
    repaint();
  }

  void closeSettings() noexcept
  {
    mainPanel.setVisible(true);
    header.getSettingsButton().setVisible(true);
    header.getSettingsExitButton().setVisible(false);
    repaint();
  }

private:
  AbstractPanel& mainPanel;
  Header header;
};
} // namespace window
} // namespace gui
} // namespace dmt