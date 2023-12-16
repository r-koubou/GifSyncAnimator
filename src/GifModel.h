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

        juce::MemoryBlock& getGifData() noexcept;

    private:
        void reset();

#pragma region Gif loading
        bool loadGifImpl( void* gifData, size_t gifSize );
        void gifFrameWriter( const GIF_WHDR& whdr );
        static void gifFrameCallback( void* data, struct GIF_WHDR* whdr );
#pragma endregion

    };
}
