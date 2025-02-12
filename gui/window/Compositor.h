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
  const int headerHeight = dmt::Settings::Header::height;

public:
  Compositor(juce::String titleText, AbstractPanel& mainPanel)
    : mainPanel(mainPanel)
    , header(titleText)
  {
    addAndMakeVisible(header);
    addAndMakeVisible(mainPanel);
  }

  ~Compositor() noexcept override {}

  void paint(juce::Graphics& g) noexcept override {}

  void resized() noexcept override
  {
    const auto bounds = getLocalBounds();
    auto contentBounds = bounds;
    auto headerBounds = contentBounds.removeFromTop(headerHeight * size);

    header.setBounds(headerBounds);
    mainPanel.setBounds(contentBounds);
  }

private:
  AbstractPanel& mainPanel;
  Header header;
};
} // namespace window
} // namespace gui
} // namespace dmt