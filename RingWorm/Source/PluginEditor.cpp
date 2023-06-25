/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingWormAudioProcessorEditor::RingWormAudioProcessorEditor (RingWormAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    addAndMakeVisible (depth_slider);
    addAndMakeVisible (rate_slider);
    
    depth_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    rate_slider.setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    depth_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    rate_slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "TEST_RATE", rate_slider));
    sliderAttachments.emplace_back (std::make_unique <SliderAttachment> (audioProcessor.apvts, "TEST_DEPTH", depth_slider));
}

RingWormAudioProcessorEditor::~RingWormAudioProcessorEditor()
{
}

//==============================================================================
void RingWormAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
}

void RingWormAudioProcessorEditor::resized()
{
    depth_slider.setBounds (0, 0, getWidth() * 0.25, getHeight() * 0.25);
    rate_slider.setBounds (0, depth_slider.getBottom(), getWidth() * 0.25, getHeight() * 0.25);
}
