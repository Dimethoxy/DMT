#pragma once

#include "gui/component/HeaderComponent.h"
#include "gui/panel/AbstractPanel.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {

class Compositor : public juce::Component
{
  using AbstractPanel = dmt::gui::panel::AbstractPanel;
  using HeaderComponent = dmt::gui::component::HeaderComponent;

  // Window size
  const float& size = dmt::Settings::Window::size;
  const int headerHeight = dmt::Settings::Window::headerHeight;

public:
  Compositor(AbstractPanel& mainPanel)
    : mainPanel(mainPanel)
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
  HeaderComponent header;
};
} // namespace window
} // namespace gui
} // namespace dmt