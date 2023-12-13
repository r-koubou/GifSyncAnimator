/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "Context.h"
#include "GifAnimationComponent.h"

//==============================================================================
/**
*/
class GifSyncAnimatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GifSyncAnimatorAudioProcessorEditor (GifSyncAnimatorAudioProcessor&);
    ~GifSyncAnimatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GifSyncAnimatorAudioProcessor& audioProcessor;

    rkoubou::GifSync::Context* context;
    rkoubou::GifSync::GifAnimationComponent* gifComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GifSyncAnimatorAudioProcessorEditor)
};
