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
class RingWormAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RingWormAudioProcessorEditor (RingWormAudioProcessor&);
    ~RingWormAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RingWormAudioProcessor& audioProcessor;
    
    juce::Slider hi_l_depth_slider, hi_r_depth_slider, lo_l_depth_slider, lo_r_depth_slider;
    juce::Slider hi_l_rate_slider, hi_r_rate_slider, lo_l_rate_slider, lo_r_rate_slider;
    
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingWormAudioProcessorEditor)
};
