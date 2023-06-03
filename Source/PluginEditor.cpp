/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModulatorAudioProcessorEditor::RingModulatorAudioProcessorEditor (RingModulatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    frequencySlider(*audioProcessor.apvts.getParameter("Frequency"), " HZ"),
    depthSlider(*audioProcessor.apvts.getParameter("Depth"), "%"),
    mixSlider(*audioProcessor.apvts.getParameter("Mix"), "%"),

    frequencySliderAttachment(audioProcessor.apvts, "Frequency", frequencySlider),
    depthSliderAttachment(audioProcessor.apvts, "Depth", depthSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    for (auto* component : GetComponents())
    {
        addAndMakeVisible(component);
    }
    frequencySlider.setTextValueSuffix(" Hz");
    depthSlider.setTextValueSuffix("%");
    mixSlider.setTextValueSuffix("%");
}

RingModulatorAudioProcessorEditor::~RingModulatorAudioProcessorEditor()
{
}

//==============================================================================
void RingModulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(20u,20u,20u));

    g.setColour (juce::Colours::white);
}

void RingModulatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto waveArea = bounds.removeFromTop(bounds.getHeight() * 0.33);

    auto mixArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto depthArea = bounds.removeFromRight(bounds.getWidth() * 0.5);

    mixSlider.setBounds(mixArea);
    depthSlider.setBounds(depthArea);
    frequencySlider.setBounds(bounds);
}

std::vector<juce::Component*> RingModulatorAudioProcessorEditor::GetComponents()
{
    return
    {
        &frequencySlider,
        &depthSlider,
        &mixSlider
    };
}

