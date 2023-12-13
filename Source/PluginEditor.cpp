/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GifSyncAnimatorAudioProcessorEditor::GifSyncAnimatorAudioProcessorEditor( rkoubou::GifSync::Context& ctx )
    : AudioProcessorEditor( ctx.getProcessor() ), context( ctx )
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize( 400, 400 );

    gifComponent = new rkoubou::GifSync::GifAnimationComponent( context, *this );

    addAndMakeVisible( gifComponent );

    gifComponent->startTimer( 1 );
}

GifSyncAnimatorAudioProcessorEditor::~GifSyncAnimatorAudioProcessorEditor()
{
    delete gifComponent;
}

//==============================================================================
void GifSyncAnimatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GifSyncAnimatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
