/*
-----------------------------------------------------------------------
Copyright (C) 2023 R-Koubou

This file is part of GifSyncAnimator.

GifSyncAnimator is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

GifSyncAnimator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GifSyncAnimator. If not, see <https://www.gnu.org/licenses/>.
-----------------------------------------------------------------------
*/

#include "GifModel.h"

namespace rkoubou::GifSync
{

    GifModel::GifModel( const juce::File& gifFile )
    {
        loaded = false;
        width = 0;
        height = 0;

        try
        {
            gifData = std::make_unique<juce::MemoryBlock>();

            if( !gifFile.existsAsFile() )
            {
                return;
            }

            bool result = gifFile.loadFileAsData( *gifData );

            if( !result )
            {
                return;
            }

            loaded = loadGifImpl( gifData->getData(), gifData->getSize() );
        }
        catch( ... )
        {
            reset();
        }
    }

    GifModel::GifModel( juce::MemoryBlock& gif )
    {
        loaded = false;
        width = 0;
        height = 0;

        try
        {
            gifData = std::make_unique<juce::MemoryBlock>( gif );

            loaded = loadGifImpl( gifData->getData(), gifData->getSize() );
        }
        catch( ... )
        {
            reset();
        }
    }

    GifModel::~GifModel() {}

    bool GifModel::isLoaded() const noexcept
    {
        return loaded;
    }

    juce::Image& GifModel::getFrameImage( int index )
    {
        return images[ index ];
    }

    int GifModel::getFrameTime( int index )
    {
        return animationTime[ index ];
    }

    int GifModel::getWidth() const noexcept
    {
        return width;
    }

    int GifModel::getHeight() const noexcept
    {
        return height;
    }

    int GifModel::getFrameCount() const noexcept
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

    juce::MemoryBlock& GifModel::getGifData() noexcept
    {
        return *gifData;
    }

    void GifModel::reset()
    {
        loaded = false;
        images.clear();
        animationTime.clear();
    }

#pragma region Gif loading

    bool GifModel::loadGifImpl( void* gifData, size_t gifSize )
    {
        long result = GIF_Load(
            gifData,                        // Memory data source
            (long)gifSize,                  // size of gif data
            GifModel::gifFrameCallback,     // the frame writer callback
            nullptr,                        // pointer to metadata
            this,                           // as void* data
            0                               // skip frames
        );

        if( result == 0 )
        {
            reset();
            return false;
        }

        return true;
    }

    void GifModel::gifFrameWriter( const GIF_WHDR& whdr )
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

    void GifModel::gifFrameCallback( void* data, struct GIF_WHDR* whdr )
    {
        reinterpret_cast<GifModel*>(data)->gifFrameWriter( *whdr );
    }

#pragma endregion

}
