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

#include "AnimationScale.h"
#include "RenderingScale.h"

namespace rkoubou::GifSync
{
    class IContextAccessor
    {
    public:
        virtual ~IContextAccessor() = default;
    public:
        virtual AnimationScale getAnimationScale() const = 0;
        virtual void setAnimationScale( AnimationScale scale ) = 0;

        virtual RenderingScale getRenderingScale() const = 0;
        virtual void setRenderingScale( RenderingScale scale ) = 0;
    };
}
