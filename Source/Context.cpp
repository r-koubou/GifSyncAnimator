#include "Context.h"

namespace rkoubou::GifSync
{
    Context::Context( juce::AudioProcessor& processor ) :
        processor( processor )
    {
    }

    Context::~Context() {}

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

    bool Context::isLoaded() const
    {
        return loaded;
    }
}
