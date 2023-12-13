#pragma once

#include <JuceHeader.h>

#include "GifModel.h"

namespace rkoubou::GifSync
{
    enum class AnimationScale
    {
        Default = 1,
        Faster2x,
        Faster4x,
        Faster8x,
        Faster16x,
        Faster32x,
        Faster64x,
        Slower2x,
        Slower4x,
        Slower8x,
        Slower16x,
        Slower32x,
        Slower64x,
    };

    class GifAnimator
    {
    private:
        uint32_t interval = 30;
        uint32_t currentFrame = 0;

        AnimationScale animationScale = AnimationScale::Default;

        juce::AudioProcessor& processor;
        GifModel& model;

    public:
        GifAnimator( juce::AudioProcessor& processor, GifModel& model );
        ~GifAnimator();

        int process();
        uint32_t calculateCurrentFrame( double ppq ) const;

        void setAnimationScale( AnimationScale scale );
        AnimationScale getAnimationScale() const;

        uint32_t getCurrentFrame() const;
        juce::Image& getCurrentFrameImage() const;
        int getCurrentFrameTime() const;
    };
}
