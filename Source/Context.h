#pragma once

#include <JuceHeader.h>

#include "GifModel.h"
#include "GifAnimator.h"

namespace rkoubou::GifSync
{

    class Context
    {
    private:
        bool loaded = false;
        juce::AudioProcessor& processor;
        std::unique_ptr<GifModel> gifModel;
        std::unique_ptr<GifAnimator> gifAnimator;
    public:
        Context( juce::AudioProcessor& processor );
        ~Context();

        juce::AudioProcessor& getProcessor() const;

        GifModel& getGifModel() const;
        GifAnimator& getGifAnimator() const;
        void loadGif( const juce::File& file );
        bool isLoaded() const;
    };
}
