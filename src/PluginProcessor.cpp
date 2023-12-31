/*
-----------------------------------------------------------------------
Copyright (C) 2023 R-Koubou

This file is part of GifSyncAnimator.

GifSyncAnimator is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

GifSyncAnimator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GifSyncAnimator. If not, see <https://www.gnu.org/licenses/>.
-----------------------------------------------------------------------
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GifSyncAnimatorAudioProcessor::GifSyncAnimatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
#if DEBUG && 0
    juce::File logFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("GifSyncAnimatorLog.txt");
    juce::Logger::setCurrentLogger(new juce::FileLogger(logFile, "GifSyncAnimator Logger: ", 0));
#endif

    context = new rkoubou::GifSync::Context( *this );
}

GifSyncAnimatorAudioProcessor::~GifSyncAnimatorAudioProcessor()
{
    delete context;
}

//==============================================================================
const juce::String GifSyncAnimatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GifSyncAnimatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GifSyncAnimatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GifSyncAnimatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GifSyncAnimatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GifSyncAnimatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GifSyncAnimatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GifSyncAnimatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GifSyncAnimatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void GifSyncAnimatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GifSyncAnimatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
    this->currentPpqPosition = 0.0;
}

void GifSyncAnimatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GifSyncAnimatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
#if 0
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
#else
    return true;
#endif
#endif
}
#endif

void GifSyncAnimatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
#if 0
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
#endif

    if( context->isLoaded() )
    {
        juce::AudioPlayHead::CurrentPositionInfo positionInfo;
        if( auto* playHead = getPlayHead() )
        {
            processPpqPositionWithPlayHead( *playHead );
        }
        else
        {
            processPpqPositionWithManual( buffer, midiMessages );
        }
    }
}

void GifSyncAnimatorAudioProcessor::processPpqPositionWithPlayHead( juce::AudioPlayHead& playHead )
{
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;

    playHead.getCurrentPosition( positionInfo );

    if( positionInfo.isPlaying )
    {
        context->getGifAnimator().calculateCurrentFrame( positionInfo.ppqPosition );
    }
}

void GifSyncAnimatorAudioProcessor::processPpqPositionWithManual( juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages )
{
    int numSamples = buffer.getNumSamples();

    // Check tempo, loops, jumps and other events.
    for( const auto metadata : midiMessages )
    {
        auto message = metadata.getMessage();
        if( message.isTempoMetaEvent() )
        {
            double secondsPerQuarterNote = message.getTempoSecondsPerQuarterNote();
            this->tempoInBpm = 60.0 / secondsPerQuarterNote;
        }
    }

    // Update playback position (PPQ)
    double ppqPerSample = 1.0 / (sampleRate / 60.0 / tempoInBpm / 4.0);
    this->currentPpqPosition += numSamples * ppqPerSample;

    if( context->isLoaded() )
    {
        context->getGifAnimator().calculateCurrentFrame( currentPpqPosition );
    }
}

//==============================================================================
bool GifSyncAnimatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GifSyncAnimatorAudioProcessor::createEditor()
{
    return new GifSyncAnimatorAudioProcessorEditor( *context );
}

//==============================================================================
void GifSyncAnimatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream stream( destData, true );
    context->saveState( stream );
}

void GifSyncAnimatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    if( sizeInBytes <= 0 )
    {
        return;
    }

    juce::MemoryInputStream stream( data, sizeInBytes, false );
    context->loadState( stream );
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GifSyncAnimatorAudioProcessor();
}
