#pragma once

#include <JuceHeader.h>

#include "gif_load.h"

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
        GifModel( const juce::File& gifFile )
        {
            loadGif( gifFile );
        }

        ~GifModel() {}

        bool isLoaded() const noexcept
        {
            return loaded;
        }

        juce::Image& getFrameImage( int index )
        {
            return std::move( images[ index ] );
        }

        int getFrameTime( int index )
        {
            return animationTime[ index ];
        }

        int getWidth() const noexcept
        {
            return width;
        }

        int getHeight() const noexcept
        {
            return height;
        }

        int getFrameCount() const noexcept
        {
            if( loaded )
            {
                return images.size();
            }
            else
            {
                return 0;
            }
        }

    private:

#pragma region Gif loading
        void loadGif( const juce::File& gifFile )
        {
            loaded  = false;
            width   = height = 0;
            gifData = std::make_unique<juce::MemoryBlock>();

            bool result = gifFile.loadFileAsData( *gifData );

            if( !result )
            {
                return;
            }

            GIF_Load(
                gifData->getData(),             // Memory data source
                (long)gifData->getSize(),       // size of gif data
                GifModel::gifFrameCallback,     // the frame writer callback
                nullptr,                        // pointer to metadata
                this,                           // as void* data
                0                               // skip frames
            );

            loaded = true;
        }

        static void gifFrameCallback( void* data, struct GIF_WHDR* whdr )
        {
            reinterpret_cast<GifModel*>(data)->gifFrameWriter( *whdr );
        }

        void gifFrameWriter( const GIF_WHDR& whdr )
        {
            // Simplified version of this: https://github.com/hidefromkgb/gif_load#c--c-usage-example
            auto [xdim, ydim, clrs, bkgd, tran, intr, mode, frxd, fryd, frxo, fryo, time, ifrm, nfrm, bptr, cpal] = whdr;

            width = std::max( width, (int)xdim );
            height = std::max( height, (int)ydim );

            juce::Image img = juce::Image( juce::Image::ARGB, xdim, ydim, true );

            {
                auto& [br, bg, bb] = cpal[ bkgd ];
                auto g = juce::Graphics( img );
                //g.fillAll(juce::Colour(br, bg, bb));

                if( mode == GIF_CURR )
                {
                    if( images.size() > 0 )
                    {
                        auto& prevImg = images[ images.size() - 1 ];
                        g.drawImageAt( prevImg, 0, 0 );
                    }
                }
            }

            for( int y = 0; y < fryd; ++y )
            {
                for( int x = 0; x < frxd; ++x )
                {
                    const auto idx = bptr[ y * frxd + x ];

                    if( tran != -1 && tran == (long)idx )
                    {
                        continue;
                    }
                    else
                    {
                        const auto [r, g, b] = cpal[ idx ];
                        img.setPixelAt( x + frxo, y + fryo, juce::PixelARGB( 0xFF, r, g, b ) );
                    }
                }
            }

            images.emplace_back( std::move( img ) );
            animationTime.emplace_back( time );
        }

#pragma endregion

    };
}
