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
