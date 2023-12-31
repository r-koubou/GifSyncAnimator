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

#pragma once

#include <JuceHeader.h>

#include "Context.h"

//==============================================================================
/**
*/
class GifSyncAnimatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GifSyncAnimatorAudioProcessor();
    ~GifSyncAnimatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock( juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages ) override;
    void processPpqPositionWithPlayHead( juce::AudioPlayHead& playHead );
    void processPpqPositionWithManual( juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages );

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    rkoubou::GifSync::Context *context;
    double sampleRate = 0.0;
    double currentPpqPosition = 0.0;
    double tempoInBpm = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GifSyncAnimatorAudioProcessor)
};
