
#pragma once

#include <JuceHeader.h>
#include "AppSettings.h"
#include "LookAndFeel.h"
#include "LinearSlider.h"
#include "Label.h"
#include "Unit.h"

namespace dmt
{
	class LinearSliderComponent : public juce::Component, public juce::Slider::Listener
	{
	public:
		LinearSliderComponent(
			dmt::AppSettings& a,
			juce::AudioProcessorValueTreeState& apvts,
			juce::String name,
			juce::String param,
			dmt::InfoUnit::Type unitType)
			:
			a(a),
			lnf(a),
			slider(a),
			sliderAttachment(apvts, param, slider),
			titleLabel(a, name, a.fontSizes.sliderTitleLabel),
			infoLabel(a, "Info Label", a.fontSizes.sliderInfoLabel),
			unitType(unitType)
		{
			slider.addListener(this);
			updateLabel();
			addAndMakeVisible(slider);
			addAndMakeVisible(titleLabel);
			addAndMakeVisible(infoLabel);
			setLookAndFeel(&lnf);
		}

		~LinearSliderComponent()
		{
			setLookAndFeel(nullptr);
		}
		void resized()
		{
			slider.setBoundsRelative(0.0f, 0.2f, 1.0f, 0.675f);
			titleLabel.setBoundsRelative(0.0f, 0.06f, 1.0f, 0.15f);
			infoLabel.setBoundsRelative(0.0f, 0.82f, 1.0f, 0.18f);
		}
		void sliderValueChanged(juce::Slider* slider)
		{
			updateLabel();
		}
	private:
		dmt::AppSettings& a;
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
}