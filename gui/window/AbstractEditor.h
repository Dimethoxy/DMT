#pragma once

#include "gui/window/Compositor.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace window {

template<typename PanelType>
class AbstractEditor : public juce::AudioProcessorEditor
{
  using OpenGLContext = juce::OpenGLContext;

protected:
  // Window size
  float& size = dmt::Settings::Window::size;
  const int baseWidth = 500;
  const int baseHeight = 270;

  // Window header
  const int& headerHeight = dmt::Settings::Header::height;

public:
  explicit AbstractEditor(juce::AudioProcessor& processor, PanelType& panel)
    : juce::AudioProcessorEditor(&processor)
    , compositor("DisFlux", panel)
  {
    if (OS_IS_WINDOWS || OS_IS_DARWIN || OS_IS_LINUX) {
      setResizable(true, true);
    }

    setConstraints(baseWidth, baseHeight + headerHeight);
    addAndMakeVisible(compositor);
    setResizable(true, true);
    setSize(baseWidth, baseHeight);

    // Set the callback for header visibility changes
    compositor.setHeaderVisibilityCallback([this](bool isHeaderVisible) {
      handleHeaderVisibilityChange(isHeaderVisible);
    });
  }

  ~AbstractEditor() override = default;

  void paint(juce::Graphics& g) override
  {
    // Just painting the background
    g.fillAll(dmt::Settings::Window::backgroundColour);
  }

  void resized() override
  {
    // Set the global size
    const int currentHeight = getHeight();
    const float newSize =
      (float)currentHeight /
      (compositor.isHeaderVisible() ? baseHeight + headerHeight : baseHeight);

    // Make sure the size makes sense
    if (newSize <= 0.0f || std::isinf(newSize)) {
      jassertfalse;
    }

    dmt::Settings::Window::size = newSize;

    // Set the bounds of the compositor to the bounds of the AbstractEditor
    compositor.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
  }

protected:
  void handleHeaderVisibilityChange(bool isHeaderVisible)
  {
    const int adjustedHeight =
      isHeaderVisible ? baseHeight + headerHeight : baseHeight;
    setConstraints(baseWidth, adjustedHeight);
    setSize(baseWidth, adjustedHeight);
  }

  void setConstraints(int width, int height)
  {
    if (auto* constrainer = this->getConstrainer()) {
      const auto aspectRatio = (double)width / (double)height;
      constrainer->setFixedAspectRatio(aspectRatio);
      const auto minWidth = width / 2;
      const auto minHeight = height / 2;
      const auto maxWidth = width * 4;
      const auto maxHeight = height * 4;
      constrainer->setSizeLimits(minWidth, minHeight, maxWidth, maxHeight);
    } else {
      jassertfalse; // Constrainer not set
    }
  }

private:
  dmt::gui::window::Compositor compositor;
};

} // namespace window
} // namespace gui
} // namespace dmt