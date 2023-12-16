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

namespace rkoubou::GifSync
{
    class GifAnimator
    {
    private:
        uint32_t interval = 30;
        uint32_t currentFrame = 0;

        IContextAccessor& context;

        juce::AudioProcessor& processor;
        GifModel& model;

    public:
        GifAnimator( IContextAccessor& ctx, juce::AudioProcessor& processor, GifModel& model );
        ~GifAnimator();

        int process();
        uint32_t calculateCurrentFrame( double ppq ) const;

        uint32_t getCurrentFrame() const;
        juce::Image& getCurrentFrameImage() const;
        int getCurrentFrameTime() const;
    };
}
