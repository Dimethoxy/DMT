/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeutrinoAudioProcessor::NeutrinoAudioProcessor()
	: AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
	valueTreeState(*this, nullptr, ProjectInfo::projectName, dmt::createParameterLayout())
{
}

NeutrinoAudioProcessor::~NeutrinoAudioProcessor()
{
}

//==============================================================================
const juce::String NeutrinoAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool NeutrinoAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool NeutrinoAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool NeutrinoAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double NeutrinoAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int NeutrinoAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int NeutrinoAudioProcessor::getCurrentProgram()
{
	return 0;
}

void NeutrinoAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NeutrinoAudioProcessor::getProgramName(int index)
{
	return {};
}

void NeutrinoAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void NeutrinoAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void NeutrinoAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NeutrinoAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void NeutrinoAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);

		// ..do something to the data...
	}
}

//==============================================================================
bool NeutrinoAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NeutrinoAudioProcessor::createEditor()
{
	return new NeutrinoAudioProcessorEditor(*this);
}

//==============================================================================
void NeutrinoAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	const auto state = valueTreeState.copyState();
	const auto xml = state.createXml();
	copyXmlToBinary(*xml, destData);
}

void NeutrinoAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	const auto xmlState = getXmlFromBinary(data, sizeInBytes);
	if (xmlState == nullptr)
		return;
	const auto newTree = juce::ValueTree::fromXml(*xmlState);
	valueTreeState.replaceState(newTree);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new NeutrinoAudioProcessor();
}
