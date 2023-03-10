#pragma once

#include <JuceHeader.h>
#include "AppSettings.h"
#include "LookAndFeel.h"

namespace dmt
{
	class LinearSlider : public juce::Slider
	{
	public:
		LinearSlider(dmt::AppSettings& a)
			:
			juce::Slider(),
			a(a),
			lnf(a)
		{
			setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
			setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
			juce::Random rand;
			setValue(10.0f * rand.nextFloat());
			setLookAndFeel(&lnf);
		}
		~LinearSlider()
		{
			setLookAndFeel(nullptr);
		}
	private:
		dmt::AppSettings& a;
		dmt::LookAndFeel lnf;
	};
}