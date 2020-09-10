/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FiltertesterAudioProcessor::FiltertesterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
,
state(*this, nullptr, juce::Identifier("params"),
{
    std::make_unique<juce::AudioParameterInt>("cutoff", "Cutoff", 20, 20000, 20000),
    
    std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", juce::NormalisableRange<float>(0.1f, 5.0, 0.1f), 1.0f),
    
    std::make_unique<juce::AudioParameterInt>("type", "Type", 0, 2, 0)
}
)
#endif
{
}

FiltertesterAudioProcessor::~FiltertesterAudioProcessor()
{
}

//==============================================================================
const juce::String FiltertesterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FiltertesterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FiltertesterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FiltertesterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FiltertesterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FiltertesterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FiltertesterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FiltertesterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FiltertesterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FiltertesterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FiltertesterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    //these variables need to be initialised before updateFilter func 11/09/20 PW
    typeParam = state.getRawParameterValue("type");
    cutoffParam = state.getRawParameterValue("cutoff");
    resonanceParam = state.getRawParameterValue("resonance");
    
    stateVariableFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);
    
}

void FiltertesterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void FiltertesterAudioProcessor::updateFilter()
{
    
    if (*typeParam == 0)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency (lastSampleRate, *cutoffParam, *resonanceParam);
    }
    
    if (*typeParam == 1)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, *cutoffParam, *resonanceParam);
    }
    
    if (*typeParam == 2)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, *cutoffParam, *resonanceParam);
    }
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FiltertesterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void FiltertesterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    juce::dsp::AudioBlock<float> block (buffer);
    
    updateFilter();
    stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
    
}

//==============================================================================
bool FiltertesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FiltertesterAudioProcessor::createEditor()
{
    return new FiltertesterAudioProcessorEditor (*this, state);
}

//==============================================================================
void FiltertesterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto stateTree = state.copyState();
    std::unique_ptr<juce::XmlElement> xml (stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void FiltertesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(state.state.getType()))
    {
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FiltertesterAudioProcessor();
}
