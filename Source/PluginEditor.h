/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotarySlider_RC.h"

//==============================================================================
/**
*/
class RingModulatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor&);
    ~RingModulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::vector<juce::Component*> GetComponents();
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RingModulatorAudioProcessor& audioProcessor;
    RotarySlider frequencySlider, 
                        depthSlider,
                        mixSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment frequencySliderAttachment,
                depthSliderAttachment,
                mixSliderAttachment;

    juce::Label frequencyLabel{ "Frequency" };
    juce::Label depthLabel{ "Depth" };
    juce::Label mixLabel{ "Mix" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
