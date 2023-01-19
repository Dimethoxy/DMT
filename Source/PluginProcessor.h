/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin
 processor.
 
 ==============================================================================
 */

#pragma once

#include "PresetManager.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include <JuceHeader.h>

namespace dmt {
static juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout()
{
    return juce::AudioProcessorValueTreeState::ParameterLayout{
        
        std::make_unique<juce::AudioParameterFloat>(
                                                    "gain",
                                                    "Gain",
                                                    juce::NormalisableRange<float>(-32.0f, 0.0f, 0.2f, 1.0f),
                                                    0.0f),
        std::make_unique<juce::AudioParameterChoice>(
                                                     "waveform",
                                                     "Waveform",
                                                     juce::StringArray{ "Sine", "Saw", "Triangle", "Square", "Noise" },
                                                     0)
    };
}

class AhdEnvelope
{
    struct Parameters
    {
        float attack = 0.0f;
        float hold = 0.0f;
        float decay = 0.0f;
        int attackScew = 0.0f;
        int decayScew = 0.0f;
        };
    
    enum class State
    {
        Attack,
        Hold,
        Decay,
        Idle
    };
    
public:
    AhdEnvelope() {}
    
    void setParameters(Parameters params) { this->params = params; }
    void setSampleRate(int sampleRate) { this->sampleRate = sampleRate; }
    void noteOn() { sampleIndex = 0; }
    
    State getState()
    {
        if (sampleIndex < getHoldStart())
            return State::Attack;
        if (sampleIndex < getDecayStart())
            return State::Hold;
        return State::Decay;
    }
    
    float getNextSample()
    {
        auto state = getState();
        float result = getValue(state);
        sampleIndex++;
        return result;
    }
    
private:
    float getValue(State state){
        switch(state){
            case State::Attack:{
                float normalizedPosition = sampleIndex/sampleRate;
                float scew = getScew(State::Attack);
                float value = std::pow(normalizedPosition, scew);
                return value;
            }
            case State::Hold:{
                return 1.0f;
            }
            case State::Decay:{
                float decayStart = getDecayStart();
                float normalizedPosition = (sampleIndex - decayStart)/sampleRate;
                float scew = getScew(State::Decay);
                float value = 1.0f - std::pow(normalizedPosition, scew);
                return value;
            }
            default:{
                return 0.0f;
            }
        }
    }
    float getScew(State state){
        switch(state){
            case State::Attack:{
                return getScewAsExponent(params.attackScew);
            }
            case State::Decay:{
                return getScewAsExponent(params.decayScew);
            }
            default:{
                return 1.0f;
            }
        }
    }
    float getScewAsExponent(int rawScew){
        if (rawScew >= 0){
            return 1.0f + (rawScew * 0.09);
        }
        if (rawScew < 0){
            return 1.0f + (rawScew * 0.009);
        }
    }
    int getHoldStart() { return params.attack * sampleRate; }
    int getDecayStart() { return (params.attack + params.hold) * sampleRate; }
    int sampleRate = -1;
    Parameters params;
    int sampleIndex = 0;
};
}

//==============================================================================
class NeutrinoAudioProcessor
: public juce::AudioProcessor
#if JucePlugin_Enable_ARA
, public juce::AudioProcessorARAExtension
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
    
    //==============================================================================
    dmt::PresetManager& getPresetManager() { return *presetManager; }
    
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState valueTreeState;
    std::unique_ptr<dmt::PresetManager> presetManager;
    
    juce::Synthesiser synth;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeutrinoAudioProcessor)
};
