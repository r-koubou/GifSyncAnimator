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

#include "GifAnimationComponent.h"

namespace rkoubou::GifSync
{
    GifAnimationComponent::GifAnimationComponent( Context& context, juce::AudioProcessorEditor& editor ) :
        context( context ),
        editor( editor ),
        loading( false )
    {
        setBufferedToImage( true );
        setSize( editor.getWidth(), editor.getHeight() );
    }

    GifAnimationComponent::~GifAnimationComponent()
    {}

    void GifAnimationComponent::paint( juce::Graphics & g )
    {
        g.fillAll( juce::Colours::white );

        if( loading || !context.isLoaded() )
        {
            g.setColour( juce::Colours::black );
            g.setFont( 20 );
            g.drawFittedText( "Click this area to load gif file", getLocalBounds(), juce::Justification::centred, 1 );
            return;
        }

        g.drawImage(
            context.getGifAnimator().getCurrentFrameImage(),
            getLocalBounds().toFloat(),
            juce::RectanglePlacement::centred
        );
    }

    void GifAnimationComponent::timerCallback()
    {
        processFrame();
    }

    void GifAnimationComponent::mouseDown( const juce::MouseEvent & event )
    {
        bool isPrimaryButton = !event.mods.isPopupMenu() && !event.mods.isMiddleButtonDown();

        if( isPrimaryButton )
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Select gif file",
                juce::File{},
                "*.gif"
            );

            auto fileChosenCallback = [this]( const juce::FileChooser& chooser ){
                auto file = chooser.getResult();
                if( file.exists() )
                {
                    context.loadGif( file );

                    if(context.isLoaded() )
                    {
                        int w = context.getGifModel().getWidth();
                        int h = context.getGifModel().getHeight();

                        if( w > 0 && h > 0 )
                        {
                            setSize( w, h );
                            editor.setSize( w, h );
                            editor.setResizeLimits( w / 2, h / 2, w * 2, h * 2 );
                        }
                    }
                }
                startTimer( 1 );
            };

            stopTimer();

            fileChooser->launchAsync(
                juce::FileBrowserComponent::FileChooserFlags::openMode | juce::FileBrowserComponent::FileChooserFlags::canSelectFiles,
                fileChosenCallback
            );

            return;
        }

        RenderingScale renderScale = context.getRenderingScale();
        AnimationScale animationScale = context.getAnimationScale();

        juce::PopupMenu subMenuSpeed;
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleDefault, "Default", true,  animationScale == AnimationScale::Default );
        subMenuSpeed.addSeparator();
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster2x, "Faster 2x", true, animationScale == AnimationScale::Faster2x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster4x, "Faster 4x", true, animationScale == AnimationScale::Faster4x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster8x, "Faster 8x", true, animationScale == AnimationScale::Faster8x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster16x, "Faster 16x", true, animationScale == AnimationScale::Faster16x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster32x, "Faster 32x", true, animationScale == AnimationScale::Faster32x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleFaster64x, "Faster 64x", true, animationScale == AnimationScale::Faster64x );
        subMenuSpeed.addSeparator();
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower2x, "Slower 2x", true, animationScale == AnimationScale::Slower2x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower4x, "Slower 4x", true, animationScale == AnimationScale::Slower4x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower8x, "Slower 8x", true, animationScale == AnimationScale::Slower8x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower16x, "Slower 16x", true, animationScale == AnimationScale::Slower16x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower32x, "Slower 32x", true, animationScale == AnimationScale::Slower32x );
        subMenuSpeed.addItem( (int)PopupMenuIds::AnimationScaleSlower64x, "Slower 64x", true, animationScale == AnimationScale::Slower64x );

        juce::PopupMenu subMenuGuiScale;
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScaleDefault, "Default", true, renderScale == RenderingScale::Default );
        subMenuGuiScale.addSeparator();
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScale50, "50%", true, renderScale == RenderingScale::Scale50 );
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScale75, "75%", true, renderScale == RenderingScale::Scale75 );
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScale125, "125%", true, renderScale == RenderingScale::Scale125 );
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScale150, "150%", true, renderScale == RenderingScale::Scale150 );
        subMenuGuiScale.addItem( (int)PopupMenuIds::GuiScale200, "200%", true, renderScale == RenderingScale::Scale200 );

        juce::PopupMenu menu;
        menu.addSubMenu( "Animation Speed", subMenuSpeed );
        menu.addSubMenu( "GUI Scale", subMenuGuiScale );

        menu.showMenuAsync( juce::PopupMenu::Options(), [this]( int result ){
            //context.getGifAnimator().setAnimationScale( static_cast<AnimationScale>( result ) );
            handlePopupMenu( static_cast<PopupMenuIds>( result ) );
        });
    }

    void GifAnimationComponent::setRenderingScale( RenderingScale scale )
    {
        auto factor = 1.0;

        switch( scale )
        {
            case RenderingScale::Scale50: factor = 0.5; break;
            case RenderingScale::Scale75: factor = 0.75; break;
            case RenderingScale::Scale125: factor = 1.25; break;
            case RenderingScale::Scale150: factor = 1.5; break;
            case RenderingScale::Scale200: factor = 2.0; break;
            default:
                break;
        }

        int w = (int)( (double)context.getGifModel().getWidth() * factor );
        int h = (int)( (double)context.getGifModel().getHeight() * factor );
        setSize( w, h );
        editor.setSize( w, h );

        context.setRenderingScale( scale );
    }

    void GifAnimationComponent::handlePopupMenu( PopupMenuIds id )
    {
#pragma region AnimationScale
        if( id > PopupMenuIds::AnimationScaleBegin && id < PopupMenuIds::AnimationScaleEnd )
        {
            AnimationScale scale = AnimationScale::Default;

            switch( id )
            {
                case PopupMenuIds::AnimationScaleFaster2x: scale = AnimationScale::Faster2x; break;
                case PopupMenuIds::AnimationScaleFaster4x: scale = AnimationScale::Faster4x; break;
                case PopupMenuIds::AnimationScaleFaster8x: scale = AnimationScale::Faster8x; break;
                case PopupMenuIds::AnimationScaleFaster16x: scale = AnimationScale::Faster16x; break;
                case PopupMenuIds::AnimationScaleFaster32x: scale = AnimationScale::Faster32x; break;
                case PopupMenuIds::AnimationScaleFaster64x: scale = AnimationScale::Faster64x; break;
                case PopupMenuIds::AnimationScaleSlower2x: scale = AnimationScale::Slower2x; break;
                case PopupMenuIds::AnimationScaleSlower4x: scale = AnimationScale::Slower4x; break;
                case PopupMenuIds::AnimationScaleSlower8x: scale = AnimationScale::Slower8x; break;
                case PopupMenuIds::AnimationScaleSlower16x: scale = AnimationScale::Slower16x; break;
                case PopupMenuIds::AnimationScaleSlower32x: scale = AnimationScale::Slower32x; break;
                case PopupMenuIds::AnimationScaleSlower64x: scale = AnimationScale::Slower64x; break;
                default:
                    break;
            }
            context.setAnimationScale( scale );
            return;
        }
#pragma endregion

        if( id > PopupMenuIds::GuiScaleBegin && id < PopupMenuIds::GuiScaleEnd )
        {
            RenderingScale scale = RenderingScale::Default;

            switch( id )
            {
                case PopupMenuIds::GuiScale50: scale = RenderingScale::Scale50; break;
                case PopupMenuIds::GuiScale75: scale = RenderingScale::Scale75; break;
                case PopupMenuIds::GuiScale125: scale = RenderingScale::Scale125; break;
                case PopupMenuIds::GuiScale150: scale = RenderingScale::Scale150; break;
                case PopupMenuIds::GuiScale200: scale = RenderingScale::Scale200; break;
                default:
                    break;
            }

            setRenderingScale( scale );
        }
    }

    void GifAnimationComponent::processFrame()
    {
        if( loading || !context.isLoaded() )
        {
            startTimer( 100 );
            return;
        }

        repaint();

        if( context.isPlayHeadAvailable() )
        {
            juce::AudioPlayHead::CurrentPositionInfo positionInfo;
            if( auto* playHead = context.getProcessor().getPlayHead() )
            {
                playHead->getCurrentPosition( positionInfo );
                if( positionInfo.isPlaying )
                {
                    int time = context.getGifAnimator().process( positionInfo.ppqPosition );
                    startTimer( time );
                    return;
                }
            }
        }

        startTimer( 30 );
    }
}
