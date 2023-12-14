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

    private:
        void handlePopupMenu( PopupMenuIds id );
        void processFrame();
    };
}
