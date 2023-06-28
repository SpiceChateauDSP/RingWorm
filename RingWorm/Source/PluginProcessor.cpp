/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingWormAudioProcessor::RingWormAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor   (BusesProperties()
                        #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        #endif
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                        #endif
                        ), apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

RingWormAudioProcessor::~RingWormAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout RingWormAudioProcessor::createParameterLayout(){
    std::vector <std::unique_ptr <juce::RangedAudioParameter>> parameters;
    
    juce::NormalisableRange<float> rateRange (0.f, 10.f, 0.01f);
    juce::NormalisableRange<float> depthRange (-36.f, 0.f, 0.1f);
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"HI_L_RATE", 1}, "Hi L Rate", rateRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"HI_R_RATE", 1}, "Hi R Rate", rateRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"LO_L_RATE", 1}, "Lo L Rate", rateRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"LO_R_RATE", 1}, "LO R Rate", rateRange, 0.f));
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"HI_L_DEPTH", 1}, "Hi L Depth", depthRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"HI_R_DEPTH", 1}, "Hi R Depth", depthRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"LO_L_DEPTH", 1}, "Lo L Depth", depthRange, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"LO_R_DEPTH", 1}, "Lo R Depth", depthRange, 0.f));
    
    return { parameters.begin(), parameters.end() };
}

//==============================================================================
const juce::String RingWormAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingWormAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingWormAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingWormAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingWormAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RingWormAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingWormAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingWormAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RingWormAudioProcessor::getProgramName (int index)
{
    return {};
}

void RingWormAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RingWormAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    crossover.prepare (sampleRate);
    hi_lfo_l.prepare (sampleRate);
    hi_lfo_r.prepare (sampleRate);
    lo_lfo_l.prepare (sampleRate);
    lo_lfo_r.prepare (sampleRate);
}

void RingWormAudioProcessor::releaseResources()
{
    crossover.reset();
    hi_lfo_l.reset();
    hi_lfo_r.reset();
    lo_lfo_l.reset();
    lo_lfo_r.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingWormAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void RingWormAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto newRate = apvts.getRawParameterValue ("HI_L_RATE");
    hi_lfo_l.setRate (*newRate);
    
    newRate = apvts.getRawParameterValue ("HI_R_RATE");
    hi_lfo_r.setRate (*newRate);
    
    newRate = apvts.getRawParameterValue ("LO_L_RATE");
    lo_lfo_l.setRate (*newRate);
    
    newRate = apvts.getRawParameterValue ("LO_R_RATE");
    lo_lfo_r.setRate (*newRate);
    
    auto newDepth = apvts.getRawParameterValue ("HI_L_DEPTH");
    hi_lfo_l.setDepth (*newDepth);
    
    newDepth = apvts.getRawParameterValue ("HI_R_DEPTH");
    hi_lfo_r.setDepth (*newDepth);
    
    newDepth = apvts.getRawParameterValue ("LO_L_DEPTH");
    lo_lfo_l.setDepth (*newDepth);
    
    newDepth = apvts.getRawParameterValue ("LO_R_DEPTH");
    lo_lfo_r.setDepth (*newDepth);
    
    /* Process */
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            auto input = channelData[sample];
            
            /* Crossover */
            auto hi_out = crossover.process_high (input, channel);
            auto lo_out = crossover.process_low (input, channel);
            
            /* LFO */
            double hi_lfo_out_dB, lo_lfo_out_dB;
            double hi_lfo_out_0to1, lo_lfo_out_0to1;
            
            if (channel == 0) {
                hi_lfo_out_dB = hi_lfo_l.getCurrentLFOPosition (0);
                hi_lfo_out_0to1 = juce::Decibels::decibelsToGain (hi_lfo_out_dB);
                lo_lfo_out_dB = lo_lfo_l.getCurrentLFOPosition (0);
                lo_lfo_out_0to1 = juce::Decibels::decibelsToGain (lo_lfo_out_dB);
                
                hi_out *= hi_lfo_out_0to1;
                lo_out *= lo_lfo_out_0to1;
            } else {
                hi_lfo_out_dB = hi_lfo_r.getCurrentLFOPosition (1);
                hi_lfo_out_0to1 = juce::Decibels::decibelsToGain (hi_lfo_out_dB);
                lo_lfo_out_dB = lo_lfo_r.getCurrentLFOPosition (1);
                lo_lfo_out_0to1 = juce::Decibels::decibelsToGain (lo_lfo_out_dB);
                
                hi_out *= hi_lfo_out_0to1;
                lo_out *= lo_lfo_out_0to1;
            }
            
            auto channel_out = hi_out + lo_out;
            
            channelData[sample] = channel_out;
        }
    }
}

//==============================================================================
bool RingWormAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RingWormAudioProcessor::createEditor()
{
    return new RingWormAudioProcessorEditor (*this);
}

//==============================================================================
void RingWormAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RingWormAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingWormAudioProcessor();
}
