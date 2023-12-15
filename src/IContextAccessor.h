#pragma once

#include <JuceHeader.h>

#include "AnimationScale.h"
#include "RenderingScale.h"

namespace rkoubou::GifSync
{
    __interface IContextAccessor
    {
    public:
        virtual AnimationScale getAnimationScale() const = 0;
        virtual void setAnimationScale( AnimationScale scale ) = 0;

        virtual RenderingScale getRenderingScale() const = 0;
        virtual void setRenderingScale( RenderingScale scale ) = 0;
    };
}
