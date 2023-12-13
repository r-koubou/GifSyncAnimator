#pragma once

#include <JuceHeader.h>

#pragma region gif_load.h

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4701)
#endif

#include "gif_load.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#pragma endregion

namespace rkoubou::GifSync
{
    class GifModel
    {
    private:
        bool loaded = false;
        std::vector<juce::Image> images;
        std::vector<int> animationTime;

        std::unique_ptr<juce::MemoryBlock> gifData;
        int width;
        int height;

    public:
        GifModel( const juce::File& gifFile );
        GifModel( juce::MemoryBlock& gif );
        ~GifModel();

        bool isLoaded() const noexcept;

        juce::Image& getFrameImage( int index );

        int getFrameTime( int index );
        int getWidth() const noexcept;
        int getHeight() const noexcept;
        int getFrameCount() const noexcept;

        std::unique_ptr<juce::MemoryBlock>& getGifData() noexcept;

    private:
        void reset();

#pragma region Gif loading
        bool loadGifImpl( void* gifData, size_t gifSize );
        void gifFrameWriter( const GIF_WHDR& whdr );
        static void gifFrameCallback( void* data, struct GIF_WHDR* whdr );
#pragma endregion

    };
}
