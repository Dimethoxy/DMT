
#pragma once

#include "../Utility/AppSettings.h"
#include "../Utility/Unit.h"
#include "Label.h"
#include "LinearSlider.h"
#include "LookAndFeel.h"
#include <JuceHeader.h>

namespace dmt {
namespace gui {
namespace widgets {
class LinearSliderComponent
  : public juce::Component
  , public juce::Slider::Listener
{
  using Settings = AppSettings;

public:
  LinearSliderComponent(juce::AudioProcessorValueTreeState& apvts,
                        juce::String name,
                        juce::String param,
                        dmt::InfoUnit::Type unitType)
    : sliderAttachment(apvts, param, slider)
    , titleLabel(name,
                 Settings::Fonts::sliderTitleLabelSize,
                 Settings::Fonts::medium)
    , infoLabel("Info Label",
                Settings::Fonts::sliderInfoLabelSize,
                Settings::Fonts::regular)
    , unitType(unitType)
  {
    slider.addListener(this);
    updateLabel();
    addAndMakeVisible(slider);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(infoLabel);
    setLookAndFeel(&lnf);
  }

  ~LinearSliderComponent() { setLookAndFeel(nullptr); }
  void resized()
  {
    float labelSize = 0.15f;
    titleLabel.setBoundsRelative(0.0f, 0.0f, 1.0f, labelSize);

    float sliderY = 0.0f;
    float sliderHeight = 1.0f;
    slider.setBoundsRelative(0.0f, sliderY, 1.0f, sliderHeight);

    float infoLabelY = 1.0f - labelSize;
    infoLabel.setBoundsRelative(0.0f, infoLabelY, 1.0f, labelSize);
  }
  void sliderValueChanged(juce::Slider* slider) { updateLabel(); }

private:
  dmt::LinearSlider slider;
  juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
  dmt::Label titleLabel;
  dmt::Label infoLabel;
  dmt::LookAndFeel lnf;
  dmt::InfoUnit::Type unitType;

  void updateLabel()
  {
    auto text = dmt::InfoUnit::getString(unitType, slider.getValue());
    infoLabel.setText(text, juce::NotificationType::dontSendNotification);
  }
};
} // namespace widgets
} // namespace gui
} // namespace dmt