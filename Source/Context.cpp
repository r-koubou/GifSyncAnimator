#include "Context.h"

namespace rkoubou::GifSync
{
    Context::Context( juce::AudioProcessor& processor ) :
        processor( processor ),
        animationScale( AnimationScale::Default ),
        renderingScale( RenderingScale::Default )
    {
    }

    Context::~Context()
    {}

    juce::AudioProcessor& Context::getProcessor() const
    {
        return processor;
    }

    GifModel& Context::getGifModel() const
    {
        return *gifModel;
    }

    GifAnimator& Context::getGifAnimator() const
    {
        return *gifAnimator;
    }

    void Context::loadGif( const juce::File& file )
    {
        gifModel = std::make_unique<GifModel>( file );
        gifAnimator = std::make_unique<GifAnimator>( *this, processor, *gifModel );
        loaded = gifModel.get()->isLoaded();
    }

    void Context::loadGif( juce::MemoryBlock& block )
    {
        gifModel = std::make_unique<GifModel>( block );
        gifAnimator = std::make_unique<GifAnimator>( *this, processor, *gifModel );
        loaded = gifModel.get()->isLoaded();
    }

    bool Context::isLoaded() const
    {
        return loaded;
    }

    AnimationScale Context::getAnimationScale() const
    {
        return animationScale;
    }

    void Context::setAnimationScale( AnimationScale scale )
    {
        animationScale = scale;
    }

    RenderingScale Context::getRenderingScale() const
    {
        return renderingScale;
    }

    void Context::setRenderingScale( RenderingScale scale )
    {
        renderingScale = scale;
    }
}
