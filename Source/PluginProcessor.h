/*
  ==============================================================================

		This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace dmt
{
	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		return juce::AudioProcessorValueTreeState::ParameterLayout{

			std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float>(-32.0f, 0.0f, 0.2f, 1.0f), 0.0f),
			std::make_unique<juce::AudioParameterChoice>("waveform", "Waveform", juce::StringArray { "Sine", "Saw", "Triangle", "Square", "Noise" }, 0)
		};
	}
}
//==============================================================================
/**
 */
class NeutrinoAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
	,
	public juce::AudioProcessorARAExtension
#endif
{
public:
	//==============================================================================
	NeutrinoAudioProcessor();
	~NeutrinoAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	juce::AudioProcessorValueTreeState valueTreeState;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessor)
};
