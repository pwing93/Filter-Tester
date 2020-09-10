/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FiltertesterAudioProcessorEditor::FiltertesterAudioProcessorEditor (FiltertesterAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
    : AudioProcessorEditor (&p), audioProcessor (p), params(state)
{
    setSize (600, 200);
    
    setLookAndFeel(&otherLookAndFeel);
    
    addSlider("cutoff", "Cutoff", cutoffSlider, cutoffLabel, cutoffAttachment);
    addSlider("resonance", "Resonance", resonanceSlider, resonanceLabel, resonanceAttachment);
    addSlider("type", "Type", typeSlider, typeLabel, typeAttachment);
    
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    resonanceSlider.setSliderStyle(juce::Slider::Rotary);
    typeSlider.setSliderStyle(juce::Slider::Rotary);
    
}

FiltertesterAudioProcessorEditor::~FiltertesterAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void FiltertesterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::peru);
}

void FiltertesterAudioProcessorEditor::resized()
{
    cutoffSlider.setBounds(25, 25, 150, 150);
    resonanceSlider.setBounds(225, 25, 150,150);
    typeSlider.setBounds(425, 25, 150, 150);
}
