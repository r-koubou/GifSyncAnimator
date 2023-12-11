#pragma once

#include <JuceHeader.h>

#include "GifModel.h"

namespace rkoubou::GifSync
{

    class Context {
    private:
        std::unique_ptr<GifModel> gifModel;
    public:
        Context() {}
        ~Context() {}

        GifModel& getGifModel()
        {
            return *gifModel;
        }

        void loadGif( const juce::File& file )
        {
            gifModel = std::make_unique<GifModel>( file );
        }
    };
}
