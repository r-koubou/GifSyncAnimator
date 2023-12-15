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

#pragma region Save & Load state

static const char SIGNATURE[2] = {'G', 'S'};
#define VERSION                 1

#define JSON_VERSION            "version"
#define JSON_ANIMATION_SCALE    "animationScale"
#define JSON_RENDERING_SCALE    "renderingScale"
#define JSON_GIF_DATA           "gifdata"

/*
    [0..1] signature ('GS')
    [2..*] json

json:
    {
        "version": 1,
        "animationScale": 1,
        "renderingScale": 1,
        "gifdata": "<Base64 Encoded>..."
    }
*/

    void Context::saveState( juce::OutputStream& stream ) const
    {
        juce::DynamicObject::Ptr json = new juce::DynamicObject();

        json->setProperty( JSON_VERSION, 1 );
        json->setProperty( JSON_ANIMATION_SCALE, static_cast<int>( animationScale ) );
        json->setProperty( JSON_RENDERING_SCALE, static_cast<int>( renderingScale ) );

        juce::Logger::writeToLog( "save: animationScale: " + juce::String( static_cast<int>( animationScale ) ) );
        juce::Logger::writeToLog( "save: renderingScale: " + juce::String( static_cast<int>( renderingScale ) ) );

        if( isLoaded() )
        {
            juce::MemoryBlock& gif = gifModel->getGifData();
            json->setProperty( JSON_GIF_DATA, gif.toBase64Encoding() );
        }
        else
        {
            json->setProperty( JSON_GIF_DATA, juce::var() );
        }

        stream.write( SIGNATURE, 2 );

        juce::String jsonString = juce::JSON::toString( juce::var( json.get() ), true );
        stream.write( jsonString.toRawUTF8(), jsonString.getNumBytesAsUTF8() );
    }

    void Context::loadState( juce::InputStream & stream )
    {
        char signature[ 2 ];
        stream.read( signature, 2 );

        if( signature[ 0 ] != SIGNATURE[ 0 ] ||
            signature[ 1 ] != SIGNATURE[ 1 ] )
        {
            return;
        }

        juce::DynamicObject::Ptr json = juce::JSON::parse( stream ).getDynamicObject();

        int version = json->getProperty( JSON_VERSION );
        if( version != VERSION )
        {
            return;
        }

        animationScale = static_cast<AnimationScale>( static_cast<int>( json->getProperty( JSON_ANIMATION_SCALE ) ) );
        renderingScale = static_cast<RenderingScale>( static_cast<int>( json->getProperty( JSON_RENDERING_SCALE ) ) );

        juce::Logger::writeToLog( "load: animationScale: " + juce::String( static_cast<int>( animationScale ) ) );
        juce::Logger::writeToLog( ":oad: renderingScale: " + juce::String( static_cast<int>( renderingScale ) ) );

        juce::var gif = json->getProperty( JSON_GIF_DATA );

        if( gif.isVoid() )
        {
            return;
        }

        juce::String gifBase64 = gif.toString();
        juce::MemoryBlock gifData = juce::MemoryBlock();

        gifData.fromBase64Encoding( gifBase64 );
        loadGif( gifData );
    }
#pragma endregion

}
