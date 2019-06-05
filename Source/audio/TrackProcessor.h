#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "AbstractProcessor.h"

#include <array>

namespace audioplayer {
    class TrackProcessor : AbstractProcessor {
    public:
        TrackProcessor();
        ~TrackProcessor();
        
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void process(juce::AudioBuffer<float> &buffer) override;
        void releaseResources() override;

        void setGain(float);
        float getGain() const;
        
        void setPan(float);
        float getPan() const;
        
        void setFilePath(const juce::String &filePath);
        const juce::String &getFilePath() const;
        
        bool getMute() const;
        void setMute(bool);
        
        bool getSolo() const;
        void setSolo(bool);
        
        bool getBypass() const;
        void setBypass(bool);
        
        juce::AudioFormatManager &getFormatManager();
        
    private:
        void updateLinearGain();
        
    private:
        juce::String filePath;
        juce::AudioFormatManager formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> currentAudioFileSource;
        juce::AudioSourcePlayer audioSourcePlayer;
        juce::AudioTransportSource transportSource;
        
        bool shouldUpdate{false};
        float gain{0.f}; // dB
        float pan{0.f};
        std::array<float, 2> linearGain {{0.f, 0.f}}; // Only stereo
        std::array<float, 2> linearGainPrevious {{0.f, 0.f}};// Only stereo
        
        bool mute{false};
        bool solo{false};
        bool bypass{true};
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackProcessor)
    };
} // namespace audioplayer
