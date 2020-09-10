/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel()
    {
        setColour (juce::Slider::thumbColourId, juce::Colours::red);
    };
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
        auto centreX = (float) x + (float) width * 0.5f;
        auto centreY = (float) y + (float) height *0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        
        g.setColour (juce::Colours::royalblue);
        g.fillEllipse (rx, ry, rw, rw);
        
        g.setColour (juce::Colours::linen);
        g.drawEllipse (rx, ry, rw, rw, 3.0F);
        
        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 10.0f;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        
        g.setColour (juce::Colours::linen);
        g.fillPath (p);
    }
};


class FiltertesterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FiltertesterAudioProcessorEditor (FiltertesterAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~FiltertesterAudioProcessorEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FiltertesterAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& params;
    juce::Slider cutoffSlider, resonanceSlider, typeSlider;
    juce::Label cutoffLabel, resonanceLabel, typeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment, resonanceAttachment, typeAttachment;
    
    OtherLookAndFeel otherLookAndFeel;
    
    void addSlider(juce::String name, juce::String labelText, juce::Slider& slider, juce::Label& label, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
    
    {
        addAndMakeVisible(slider);
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment (params, name, slider));
        label.setText(labelText, juce::dontSendNotification);
        label.attachToComponent(&slider, false);
        label.setFont(25);
        label.juce::Label::setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
        slider.setPopupDisplayEnabled(true, true, this);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FiltertesterAudioProcessorEditor)
};
