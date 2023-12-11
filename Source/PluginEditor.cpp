/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GifSyncAnimatorAudioProcessorEditor::GifSyncAnimatorAudioProcessorEditor (GifSyncAnimatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    context = new rkoubou::GifSync::Context();
    context->loadGif( juce::File( "C:/UserData/Develop/Project/JUCE/MyFirstProject/giphy.gif" ) );
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

GifSyncAnimatorAudioProcessorEditor::~GifSyncAnimatorAudioProcessorEditor()
{
    delete context;
}

//==============================================================================
void GifSyncAnimatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GifSyncAnimatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
