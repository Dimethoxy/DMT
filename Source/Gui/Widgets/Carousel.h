/*
  ==============================================================================

    Carousel.h
    Created: 15 Oct 2023 3:12:19pm
    Author:  Lunix

  ==============================================================================
*/

#pragma once

#include "../Panels/Panel.h"
#include <JuceHeader.h>

//==============================================================================
namespace dmt {
namespace gui {
namespace panels {
//==============================================================================
class Carousel : public juce::Component
{
public:
  void next()
  {
    panels[index]->setVisible(false);
    index++;
    if (index >= panels.size())
      index -= panels.size();
    panels[index]->setVisible(true);
  }
  void previous()
  {
    panels[index]->setVisible(false);
    index--;
    if (index < 0)
      index += panels.size();
    panels[index]->setVisible(true);
  }
  void set(int i)
  {
    for (const auto& p : panels) {
      p->setVisible(false);
    }
    index = std::clamp(i, 0, (int)panels.size() - 1);
    panels[index]->setVisible(true);
  }

  void init()
  {
    for (auto& panel : panels) {
      panel->setCallbacks([this]() { next(); }, [this]() { previous(); });
      addAndMakeVisible(*panel);
    }
  }

  void resized() override
  {
    for (auto& panel : panels) {
      panel->setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    }
  }

protected:
  std::vector<std::unique_ptr<Panel>> panels;

private:
  int index = 0;
};
//==============================================================================
} // namespace gui
} // namespace dmt
} // namespace panels