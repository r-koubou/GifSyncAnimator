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
    GifSyncAnimatorAudioProcessorEditor( rkoubou::GifSync::Context& );
    ~GifSyncAnimatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    rkoubou::GifSync::Context& context;
    rkoubou::GifSync::GifAnimationComponent* gifComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GifSyncAnimatorAudioProcessorEditor)
};
