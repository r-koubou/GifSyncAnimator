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

    int GifAnimator::process()
    {
        if( !model.isLoaded() || model.getFrameCount() == 0 )
        {
            return interval;
        }

        juce::AudioPlayHead::CurrentPositionInfo positionInfo;
        if( auto* playHead = processor.getPlayHead() )
        {
            playHead->getCurrentPosition( positionInfo );
            if( positionInfo.isPlaying )
            {
                currentFrame = calculateCurrentFrame( positionInfo.ppqPosition );
            }
            return interval;
        }

        int result = model.getFrameTime( currentFrame );
        currentFrame++;
        currentFrame %= model.getFrameCount();

        return result * 10;
    }

    uint32_t GifAnimator::calculateCurrentFrame( double ppq ) const
    {
        const auto frameSize = model.getFrameCount();

        if (frameSize <= 0)
        {
            return 0;
        }

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

        return newFrame;
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
