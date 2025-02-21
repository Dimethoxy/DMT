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
  Compositor(juce::String titleText, AbstractPanel& mainPanel)
    : mainPanel(mainPanel)
    , header(titleText)
  {
    addAndMakeVisible(header);
    addAndMakeVisible(mainPanel);
    header.getSettingsButton().onClick = [this] { showSettings(); };
  }

  ~Compositor() noexcept override {}

  void paint(juce::Graphics& g) noexcept override {}

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();

    const auto headerHeight = rawHeaderHeight * size;
    const auto headerBounds =
      juce::Rectangle(bounds).removeFromTop(headerHeight);

    header.setBounds(headerBounds);
    mainPanel.setBounds(contentBounds);
  }

  void showSettings() noexcept
  {
    mainPanel.setVisible(false);
    header.getSettingsButton().setVisible(false);
    repaint();
  }

private:
  AbstractPanel& mainPanel;
  Header header;
};
} // namespace window
} // namespace gui
} // namespace dmt