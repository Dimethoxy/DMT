#pragma once

#include <JuceHeader.h>
#include "AppSettings.h"

namespace dmt
{
	class Label : public juce::Label
	{
	public:
		Label(dmt::AppSettings& a, juce::String name, int fontSize) :
			a(a),
			fontSize(fontSize)
		{
			setText(name, juce::NotificationType::dontSendNotification);
			setJustificationType(juce::Justification::centredTop);
			setColour(juce::Label::ColourIds::textColourId, a.colours.font);
			setFont(getFont());
		}
		Label(dmt::AppSettings& a, juce::String name, int fontSize, bool isTitle)
			: Label(a, name, fontSize)
		{
			this->isTitle = isTitle;
		}
		void resized()
		{
			setFont(getFont().withHeight(fontSize * a.size));
		}
	private:
		dmt::AppSettings& a;
		int fontSize;
		bool isTitle = false;
		juce::Font getFont()
		{
			juce::Typeface::Ptr typeface;
			if (!isTitle)
			{
				typeface = juce::Typeface::createSystemTypefaceFor(
					BinaryData::PoppinsRegular_ttf,
					BinaryData::PoppinsRegular_ttfSize);
			}
			else
			{
				typeface = juce::Typeface::createSystemTypefaceFor(
					BinaryData::SedgwickAveDisplayRegular_ttf,
					BinaryData::SedgwickAveDisplayRegular_ttfSize);
			}
			return juce::Font(typeface);
		}

	};
}