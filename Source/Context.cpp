#include "Context.h"

namespace rkoubou::GifSync
{
    Context::Context( juce::AudioProcessor& processor ) :
        processor( processor )
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
        gifAnimator = std::make_unique<GifAnimator>( processor, *gifModel );
        loaded = gifModel.get()->isLoaded();
    }

    void Context::loadGif( juce::MemoryBlock& block )
    {
        gifModel = std::make_unique<GifModel>( block );
        gifAnimator = std::make_unique<GifAnimator>( processor, *gifModel );
        loaded = gifModel.get()->isLoaded();
    }

    bool Context::isLoaded() const
    {
        return loaded;
    }
}
