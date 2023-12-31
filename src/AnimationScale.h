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

namespace rkoubou::GifSync
{
    enum class AnimationScale
    {
        Default = 1,
        Faster2x,
        Faster4x,
        Faster8x,
        Faster16x,
        Faster32x,
        Faster64x,
        Slower2x,
        Slower4x,
        Slower8x,
        Slower16x,
        Slower32x,
        Slower64x,
    };
}
