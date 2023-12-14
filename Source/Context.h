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
    };
}
