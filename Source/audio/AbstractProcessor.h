#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include <array>

namespace audioplayer {
    class AbstractProcessor {
    public:
        AbstractProcessor();
        virtual ~AbstractProcessor();
        
        virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) = 0;
        virtual void process(AudioBuffer<float> &buffer) = 0;
        virtual void releaseResources() = 0;
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbstractProcessor)
    };
} // namespace audioplayer
