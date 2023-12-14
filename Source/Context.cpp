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

    void Context::saveState( juce::OutputStream& stream ) const
    {
        /*
            bool              | gif data written flag
            int64 (BigEndian) | gif size
            void*             | gif data
        */

        if( !isLoaded() )
        {
            stream.writeBool( false ); // gif not loaded yet
            return;
        }

        uint64_t size = getGifModel().getGifData()->getSize();
        void* gifData = getGifModel().getGifData()->getData();

        stream.writeBool( true ); // gif loaded
        stream.writeInt64BigEndian( size );
        stream.write( gifData, size );
    }

    void Context::loadState( juce::InputStream & stream )
    {
        bool written = stream.readBool();

        if( !written )
        {
            return;
        }

        uint64_t size = stream.readInt64BigEndian();

        if( size == 0 )
        {
            return;
        }

        void* gif = new uint8_t[ size ];
        auto readBytes = stream.read( gif, size );

        juce::MemoryBlock gifData( gif, size );
        delete[] gif;

        loadGif( gifData );
    }
}
