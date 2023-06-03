/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModulatorAudioProcessorEditor::RingModulatorAudioProcessorEditor(RingModulatorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    waveDisplay(audioProcessor.apvts, "Modulation Wave", 0.9f),
    frequencySlider(*audioProcessor.apvts.getParameter("Frequency"), "Frequency", " HZ"),
    depthSlider(*audioProcessor.apvts.getParameter("Depth"), "Depth", "%"),
    mixSlider(*audioProcessor.apvts.getParameter("Mix"), "Mix", "%"),

    waveSelector(*audioProcessor.apvts.getParameter("Modulation Wave")),

    frequencySliderAttachment(audioProcessor.apvts, "Frequency", frequencySlider),
    depthSliderAttachment(audioProcessor.apvts, "Depth", depthSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider),

    waveSelectorAttachment(audioProcessor.apvts, "Modulation Wave", waveSelector)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    for (auto* component : GetComponents())
    {
        addAndMakeVisible(component);
    }

}

RingModulatorAudioProcessorEditor::~RingModulatorAudioProcessorEditor()
{
}

//==============================================================================
void RingModulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(20u,20u,20u));

    g.setColour(juce::Colour(40u,40u,40u));
    juce::Path p;

    //wave area
    auto newWaveArea = waveArea;
    newWaveArea.setSize(waveArea.getWidth() - 10, waveArea.getHeight() - 10);
    newWaveArea.setCentre(waveArea.getCentre());
    p.addRoundedRectangle(newWaveArea, 20.f);
    g.fillPath(p);

    //parameter area
    auto newParameterArea = parameterArea;
    newParameterArea.setSize(parameterArea.getWidth() - 10, parameterArea.getHeight() - 10);
    newParameterArea.setCentre(parameterArea.getCentre());
    p.addRoundedRectangle(newParameterArea, 20.f);
    g.fillPath(p);
}

void RingModulatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    waveArea = bounds.removeFromTop(bounds.getHeight() * 0.4);
    parameterArea = bounds;

    auto centre = bounds.getCentre();
    bounds.setSize(bounds.getWidth() - 10, bounds.getHeight() - 10);
    bounds.setCentre(centre);
    auto waveSelectionArea = bounds.removeFromTop(bounds.getHeight() * 0.33);
    auto mixArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto depthArea = bounds.removeFromRight(bounds.getWidth() * 0.5);

    mixSlider.setBounds(mixArea);
    depthSlider.setBounds(depthArea);
    frequencySlider.setBounds(bounds);

    centre = waveSelectionArea.getCentre();
    waveSelectionArea.setSize(300, 50);
    waveSelectionArea.setCentre(centre);
    waveSelector.setBounds(waveSelectionArea);

    waveDisplay.setBounds(waveArea);
}

std::vector<juce::Component*> RingModulatorAudioProcessorEditor::GetComponents()
{
    return
    {
        &waveDisplay,
        &frequencySlider,
        &depthSlider,
        &mixSlider,
        &waveSelector,
    };
}

