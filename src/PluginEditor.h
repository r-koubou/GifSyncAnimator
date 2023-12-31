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

    rkoubou::GifSync::GifAnimationComponent& getGifComponent();

private:
    rkoubou::GifSync::Context& context;
    rkoubou::GifSync::GifAnimationComponent* gifComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GifSyncAnimatorAudioProcessorEditor)
};
