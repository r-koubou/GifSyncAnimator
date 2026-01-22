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

        prevDisposalMode = 0;
        prevFrameRect = {};
        hasBackgroundColour = false;
        backgroundColour = juce::Colours::transparentBlack;
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

        const int canvasWidth = (int)xdim;
        const int canvasHeight = (int)ydim;

        const int frameW = (int)frxd;
        const int frameH = (int)fryd;
        const int offX = (int)frxo;
        const int offY = (int)fryo;

        // Retrieve the background color once only (with a guard in case it cannot be retrieved)
        if( !hasBackgroundColour && cpal != nullptr && clrs > 0 && bkgd >= 0 && bkgd < clrs )
        {
            auto& [br, bg, bb] = cpal[ bkgd ];
            backgroundColour = juce::Colour( br, bg, bb );
            hasBackgroundColour = true;
        }

        juce::Image img = juce::Image( juce::Image::ARGB, canvasWidth, canvasHeight, true );
        juce::Graphics g( img );

        const int DISPOSAL_RESTORE_BACKGROUND = 2;
        const int DISPOSAL_RESTORE_PREVIOUS = 3;

        if( !images.empty() )
        {
            if( prevDisposalMode == DISPOSAL_RESTORE_PREVIOUS && images.size() >= 2 )
            {
                // Restore to the state before the previous frame (= images[size-2])
                g.drawImageAt( images[ images.size() - 2 ], 0, 0 );
            }
            else
            {
                // Normally uses the previous frame as the base
                g.drawImageAt( images.back(), 0, 0 );

                if( prevDisposalMode == DISPOSAL_RESTORE_BACKGROUND && !prevFrameRect.isEmpty() )
                {
                    // Clear the previous frame's rectangle with the background (if the background color is unknown, clear it with transparency)
                    g.setColour( hasBackgroundColour ? backgroundColour : juce::Colours::transparentBlack );
                    g.fillRect( prevFrameRect );
                }
            }
        }
        else
        {
            // First frame: If the background color is removed, fill it in (optional)
            if( hasBackgroundColour ) {
                g.fillAll( backgroundColour );
            }
        }

        auto drawRow = [&]( int srcRow, int dstRowWithinFrame )
        {
            const int dstY = dstRowWithinFrame + offY;
            if( (unsigned)dstY >= (unsigned)canvasHeight )
            {
                return;
            }

            for( int x = 0; x < frameW; ++x )
            {
                const int dstX = x + offX;
                if( (unsigned)dstX >= (unsigned)canvasWidth )
                {
                    continue;
                }

                const int idx = (int)bptr[ srcRow * frameW + x ];

                if( tran != -1 && tran == (long)idx )
                {
                    continue;
                }

                if( cpal == nullptr || clrs <= 0 || idx < 0 || idx >= clrs )
                {
                    continue;
                }

                const auto [r, gg, b] = cpal[ idx ];
                img.setPixelAt( dstX, dstY, juce::PixelARGB( 0xFF, r, gg, b ) );
            }
        };

        if( intr == 0 )
        {
            for( int y = 0; y < frameH; ++y ) {
                drawRow( y, y );
            }
        }
        else
        {
            // GIF interlace: 4-pass (start, step) = (0,8),(4,8),(2,4),(1,2)
            int srcRow = 0;
            const int starts[ 4 ] = { 0, 4, 2, 1 };
            const int steps[ 4 ] = { 8, 8, 4, 2 };

            for( int pass = 0; pass < 4; ++pass )
            {
                for( int dst = starts[ pass ]; dst < frameH && srcRow < frameH; dst += steps[ pass ] )
                {
                    drawRow( srcRow, dst );
                    ++srcRow;
                }
            }
        }

        images.emplace_back( std::move( img ) );
        animationTime.emplace_back( (int)time );

        // ---- 次フレーム用に「このフレームのdisposal」を保存 ----
        prevDisposalMode = (int)mode;
        prevFrameRect = juce::Rectangle<int>( offX, offY, frameW, frameH );

/*
        for( int y = 0; y < fryd; ++y )
        {
            for( int x = 0; x < frxd; ++x )
            {
                const int dstX = x + (int)frxo;
                const int dstY = y + (int)fryo;

                if( (unsigned)dstX >= (unsigned)canvasWidth ||
                    (unsigned)dstY >= (unsigned)canvasHeight )
                {
                    continue;
                }

                //const auto idx = (int)bptr[y * (int)frxd + x];
                const auto idx = (int)bptr[ dstY * canvasWidth + dstX ];

                if( tran != -1 && tran == (long)idx )
                {
                    continue;
                }

                if( cpal == nullptr || clrs <= 0 || idx < 0 || idx >= clrs )
                {
                    continue;
                }

                const auto [r, gg, b] = cpal[ idx ];
                img.setPixelAt( dstX, dstY, juce::PixelARGB( 0xFF, r, gg, b ) );
            }
        }

        images.emplace_back( std::move( img ) );
        animationTime.emplace_back( (int)time );
*/

        // {
        //     auto& [br, bg, bb] = cpal[ bkgd ];
        //     auto g = juce::Graphics( img );
        //     //g.fillAll(juce::Colour(br, bg, bb));

        //     if( mode == GIF_CURR )
        //     {
        //         if( images.size() > 0 )
        //         {
        //             auto& prevImg = images[ images.size() - 1 ];
        //             g.drawImageAt( prevImg, 0, 0 );
        //         }
        //     }
        // }

        // for( int y = 0; y < fryd; ++y )
        // {
        //     for( int x = 0; x < frxd; ++x )
        //     {
        //         const auto idx = bptr[ y * frxd + x ];

        //         if( tran != -1 && tran == (long)idx )
        //         {
        //             continue;
        //         }
        //         else
        //         {
        //             const auto [r, g, b] = cpal[ idx ];
        //             img.setPixelAt( x + frxo, y + fryo, juce::PixelARGB( 0xFF, r, g, b ) );
        //         }
        //     }
        // }

        // images.emplace_back( std::move( img ) );
        // animationTime.emplace_back( time );
    }

    void GifModel::gifFrameCallback( void* data, struct GIF_WHDR* whdr )
    {
        reinterpret_cast<GifModel*>(data)->gifFrameWriter( *whdr );
    }

#pragma endregion

}
