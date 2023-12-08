/*
  ==============================================================================

        This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeutrinoAudioProcessor::NeutrinoAudioProcessor()
  : AudioProcessor(BusesProperties().withOutput("Output",
                                                juce::AudioChannelSet::stereo(),
                                                true))
  , apvts(*this,
          nullptr,
          ProjectInfo::projectName,
          dmt::createParameterLayout())
{
  apvts.state.setProperty(
    dmt::gui::preset::PresetManager::presetNameProperty, "", nullptr);
  apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);
  presetManager = std::make_unique<dmt::gui::preset::PresetManager>(apvts);

  synth.addSound(new dmt::dsp::synth::SynthSound());
  synth.addVoice(new dmt::dsp::synth::SynthVoice());
}
NeutrinoAudioProcessor::~NeutrinoAudioProcessor() {}
//==============================================================================
void
NeutrinoAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  synth.setCurrentPlaybackSampleRate(sampleRate);

  for (int i = 0; i < synth.getNumVoices(); i++) {
    auto voice = dynamic_cast<dmt::dsp::synth::SynthVoice*>(synth.getVoice(i));
    voice->prepareToPlay(sampleRate, samplesPerBlock, 2);
    voice->addOnNoteReceivers([this]() { this->filterProcessor.onNote(); });
  }

  filterProcessor.prepare(sampleRate);
}
void
NeutrinoAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  keyboardState.processNextMidiBuffer(
    midiMessages, 0, buffer.getNumSamples(), true);

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  dmt::ChainSettings chainSettings(apvts);

  if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(0))) {
    auto* ref = dynamic_cast<dmt::dsp::synth::SynthVoice*>(synth.getVoice(0));
    ref->setChainSettings(chainSettings);
  }

  synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

  // filterProcessor.setChainSettings(chainSettings);
  // filterProcessor.processBlock(buffer, 0, buffer.getNumSamples());
}
//==============================================================================
void
NeutrinoAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
  const auto state = apvts.copyState();
  const auto xml = state.createXml();
  copyXmlToBinary(*xml, destData);
}
void
NeutrinoAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  const auto xmlState = getXmlFromBinary(data, sizeInBytes);
  if (xmlState == nullptr)
    return;
  const auto newTree = juce::ValueTree::fromXml(*xmlState);
  apvts.replaceState(newTree);
}
//==============================================================================
const juce::String
NeutrinoAudioProcessor::getName() const
{
  return juce::String("Neutrino");
}

bool
NeutrinoAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool
NeutrinoAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool
NeutrinoAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double
NeutrinoAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int
NeutrinoAudioProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs,
  // so this should be at least 1, even if you're not really implementing
  // programs.
}

int
NeutrinoAudioProcessor::getCurrentProgram()
{
  return 0;
}

void
NeutrinoAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String
NeutrinoAudioProcessor::getProgramName(int /*index*/)
{
  return {};
}

void
NeutrinoAudioProcessor::changeProgramName(int /*index*/,
                                          const juce::String& /*newName*/)
{
}

//==============================================================================
void
NeutrinoAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool
NeutrinoAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

//==============================================================================
bool
NeutrinoAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor*
NeutrinoAudioProcessor::createEditor()
{
  return new NeutrinoAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
  return new NeutrinoAudioProcessor();
}
