/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum class WaveType
{
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
};
struct Settings
{
    float frequency{ 0 }, depth{ 1 }, mix{ 1 };
    WaveType waveType{ WaveType::SINE };
};

Settings GetSettings(juce::AudioProcessorValueTreeState& apvts);
//==============================================================================
/**
*/
class RingModulatorAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    RingModulatorAudioProcessor();
    ~RingModulatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", CreateParameterLayout() };

private:
    //=============================================================================
    double GetModulationSignal(const WaveType& type, const float& currentPhase) const;

    double GetSineSignal(const float& currentPhase) const;
    double GetSquareSignal(const float& currentPhase) const;
    double GetTriangleSignal(const float& currentPhase) const;
    double GetSawtoothSignal(const float& currentPhase) const;
    //==============================================================================
    double sampleRate{ 0.0 };
    double phase{ 0.0 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingModulatorAudioProcessor)

};
