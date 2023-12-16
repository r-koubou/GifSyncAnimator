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

#include "IContextAccessor.h"

#include "GifModel.h"
#include "GifAnimator.h"

namespace rkoubou::GifSync
{

    class Context : public IContextAccessor
    {
    private:
        bool loaded = false;
        juce::AudioProcessor& processor;
        std::unique_ptr<GifModel> gifModel;
        std::unique_ptr<GifAnimator> gifAnimator;

        AnimationScale animationScale;
        RenderingScale renderingScale;

    public:
        Context( juce::AudioProcessor& processor );
        ~Context();

        juce::AudioProcessor& getProcessor() const;

        GifModel& getGifModel() const;
        GifAnimator& getGifAnimator() const;
        void loadGif( const juce::File& file );
        void loadGif( juce::MemoryBlock& block );

        bool isLoaded() const;

        AnimationScale getAnimationScale() const override;
        void setAnimationScale( AnimationScale scale ) override;

        RenderingScale getRenderingScale() const override;
        void setRenderingScale( RenderingScale scale ) override;

        void saveState( juce::OutputStream& stream ) const;
        void loadState( juce::InputStream& stream );
    };
}
