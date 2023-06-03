/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveModulationUtility.h"

//==============================================================================
RingModulatorAudioProcessor::RingModulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

RingModulatorAudioProcessor::~RingModulatorAudioProcessor()
{
}

//==============================================================================
const juce::String RingModulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingModulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingModulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingModulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingModulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RingModulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingModulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingModulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RingModulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void RingModulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RingModulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    this->sampleRate = sampleRate;
    this->phase = 0.0;
}

void RingModulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingModulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void RingModulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    Settings settings = GetSettings(this->apvts);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        // Calculate the modulation signal
        double modulationSignal = Wave::GetModulationSignal(settings.waveType, phase);

        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto* outputData = buffer.getWritePointer(channel);
            const float* channelData = buffer.getReadPointer(channel);

            // Apply ring modulation to the input sample
            float modulatedSample = channelData[i] * (settings.depth * modulationSignal);

            // Mix original and modulated sample based on the mix parameter
            outputData[i] = ((1.0f - settings.mix) * channelData[i]) + (settings.mix * modulatedSample);
        }

        // Update the phase for the next sample
        double phaseIncrement = settings.frequency / sampleRate;
        phase += phaseIncrement;
        if (phase >= juce::MathConstants<float>::twoPi)
            phase -= juce::MathConstants<float>::twoPi;
    }
}

//==============================================================================
bool RingModulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RingModulatorAudioProcessor::createEditor()
{
    return new RingModulatorAudioProcessorEditor (*this);
}

//==============================================================================
void RingModulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void RingModulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

Settings GetSettings(juce::AudioProcessorValueTreeState& apvts)
{
    Settings settings;

    settings.frequency = apvts.getRawParameterValue("Frequency")->load();
    settings.depth = apvts.getRawParameterValue("Depth")->load() / 100.f;
    settings.mix = apvts.getRawParameterValue("Mix")->load() / 100.f;
    settings.waveType = (Wave::WaveType)apvts.getRawParameterValue("Modulation Wave")->load();
    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout RingModulatorAudioProcessor::CreateParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "Frequency", 
        "Frequency", 
        juce::NormalisableRange<float>(0.0f, 2000.0f, 0.1f, 1.0f), 
        1.0f));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        "Depth",
        "Depth",
        0,
        100,
        100));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        "Mix",
        "Mix",
        0,
        100,
        100));

    juce::StringArray modulationWaves;
    modulationWaves.add("Sine Wave");
    modulationWaves.add("Square Wave");
    modulationWaves.add("Triangle Wave");
    modulationWaves.add("Sawtooth Wave");

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "Modulation Wave",
        "Modulation Wave",
        modulationWaves,
        0));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModulatorAudioProcessor();
}

