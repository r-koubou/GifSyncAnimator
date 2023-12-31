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

#include "IContextAccessor.h"
#include "Context.h"

namespace rkoubou::GifSync
{
    class GifAnimationComponent : public juce::Component, public juce::Timer
    {
#pragma region PopupMenu Ids
    private:
        enum class PopupMenuIds
        {
            AnimationScaleBegin = 1,
            AnimationScaleDefault,
            AnimationScaleFaster2x,
            AnimationScaleFaster4x,
            AnimationScaleFaster8x,
            AnimationScaleFaster16x,
            AnimationScaleFaster32x,
            AnimationScaleFaster64x,
            AnimationScaleSlower2x,
            AnimationScaleSlower4x,
            AnimationScaleSlower8x,
            AnimationScaleSlower16x,
            AnimationScaleSlower32x,
            AnimationScaleSlower64x,
            AnimationScaleEnd,

            GuiScaleBegin = 100,
            GuiScaleDefault,
            GuiScale50,
            GuiScale75,
            GuiScale125,
            GuiScale150,
            GuiScale200,
            GuiScaleEnd,
        };
#pragma endregion

    private:
        const int timerInterval = (int)(1000.0 / 30.0);

        Context& context;
        juce::AudioProcessorEditor& editor;
        std::unique_ptr<juce::FileChooser> fileChooser;
        bool loading;

    public:
        GifAnimationComponent( Context& context, juce::AudioProcessorEditor& editor );
        ~GifAnimationComponent();

        void paint( juce::Graphics& g ) override;
        void timerCallback() override;
        void mouseDown( const juce::MouseEvent& event ) override;

        void setRenderingScale( RenderingScale scale );

    private:
        void handlePopupMenu( PopupMenuIds id );
        void processFrame();
    };
}
