/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotarySlider_RC.h"
#include "ComboBox_RC.h"

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

    CustomComboBox waveSelector;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SAttachment = APVTS::SliderAttachment;

    SAttachment frequencySliderAttachment,
                depthSliderAttachment,
                mixSliderAttachment;

    using CAttachment = APVTS::ComboBoxAttachment;

    CAttachment waveSelectorAttachment;

    juce::Rectangle<int> waveArea;
    juce::Rectangle<int> parameterArea;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModulatorAudioProcessorEditor)
};
