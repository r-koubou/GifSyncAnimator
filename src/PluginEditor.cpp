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
    setSize( 400, 400 );

    gifComponent = new rkoubou::GifSync::GifAnimationComponent( context, *this );

    addAndMakeVisible( gifComponent );

    if( context.isLoaded() )
    {
        juce::Logger::writeToLog( "GifSyncAnimatorAudioProcessorEditor() : scale = " + juce::String( (int)context.getRenderingScale() ) );
        gifComponent->setRenderingScale( context.getRenderingScale() );
    }

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

rkoubou::GifSync::GifAnimationComponent& GifSyncAnimatorAudioProcessorEditor::getGifComponent()
{
    return *gifComponent;
}
