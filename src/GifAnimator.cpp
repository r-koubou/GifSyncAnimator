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

#include "GifAnimator.h"

namespace rkoubou::GifSync
{
    GifAnimator::GifAnimator( IContextAccessor& ctx, juce::AudioProcessor& processor, GifModel& model ) :
        context( ctx ),
        processor( processor ),
        model( model )
    {
    }

    GifAnimator::~GifAnimator()
    {}

    void GifAnimator::calculateCurrentFrame( double ppq )
    {
        const auto frameSize = model.getFrameCount();
        double factor = 100; // 1x == 100%

        switch( context.getAnimationScale() )
        {
            case AnimationScale::Default:
                break;
            case AnimationScale::Faster2x:
                factor *= 2.0;
                break;
            case AnimationScale::Faster4x:
                factor *= 4.0;
                break;
            case AnimationScale::Faster8x:
                factor *= 8.0;
                break;
            case AnimationScale::Faster16x:
                factor *= 16.0;
                break;
            case AnimationScale::Faster32x:
                factor *= 32.0;
                break;
            case AnimationScale::Faster64x:
                factor *= 64.0;
                break;
            case AnimationScale::Slower2x:
                factor /= 2.0;
                break;
            case AnimationScale::Slower4x:
                factor /= 4.0;
                break;
            case AnimationScale::Slower8x:
                factor /= 8.0;
                break;
            case AnimationScale::Slower16x:
                factor /= 16.0;
                break;
            case AnimationScale::Slower32x:
                factor /= 32.0;
                break;
            case AnimationScale::Slower64x:
                factor /= 64.0;
                break;
            default:
                break;
        }

        int pos = (int)( ppq * factor ) % 100;
        int newFrame = ( ( frameSize * pos ) / 100 ) % frameSize;

        currentFrame = newFrame;
    }

    uint32_t GifSync::GifAnimator::getCurrentFrame() const
    {
        return currentFrame;
    }

    juce::Image& GifAnimator::getCurrentFrameImage() const
    {
        return model.getFrameImage( currentFrame );
    }

    int GifAnimator::getCurrentFrameTime() const
    {
        return model.getFrameTime( currentFrame );
    }
}
