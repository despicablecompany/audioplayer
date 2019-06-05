#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "AbstractProcessor.h"

#include <array>

namespace audioplayer {
    class MasterTrackProcessor : AbstractProcessor {
    public:
        MasterTrackProcessor();
        ~MasterTrackProcessor();
        
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void process(juce::AudioBuffer<float> &buffer) override;
        void releaseResources() override;

        void setGain(float);
        float getGain() const;
        
        void setPan(float);
        float getPan() const;
        
    private:
        void updateLinearGain();
        
    private:
        bool shouldUpdate{false};
        float gain{0.f}; // dB
        float pan{0.f};
        std::array<float, 2> linearGain {{0.f, 0.f}}; // Only stereo
        std::array<float, 2> linearGainPrevious {{0.f, 0.f}};// Only stereo
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterTrackProcessor)
    };
} // namespace audioplayer
