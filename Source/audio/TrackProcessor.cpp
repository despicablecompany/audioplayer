#include "TrackProcessor.h"

namespace audioplayer {
    TrackProcessor::TrackProcessor() {
        formatManager.registerBasicFormats();
    }
    
    TrackProcessor::~TrackProcessor() {
    }
    
    void TrackProcessor::setGain(float g) {
        if (g != gain) {
            gain = g;
            updateLinearGain();
        }
    }
    
    float TrackProcessor::getGain() const {
        return gain;
    }
    
    void TrackProcessor::setPan(float p) {
        if (p != pan) {
            pan = p;
            updateLinearGain();
        }
    }
    
    float TrackProcessor::getPan() const {
        return pan;
    }
    
    void TrackProcessor::updateLinearGain() {
        // Compute linear values
        auto g = std::pow(10.f, gain / 20.f) * M_SQRT2;
        auto p = (pan + 1.f) * M_PI_4;
        
        // Compensate for left and right channels
        linearGain[0] = g * cos(p) ; // left
        linearGain[1] = g * sin(p); // right
    }
    
    void TrackProcessor::process(juce::AudioBuffer<float> &buffer) {
        if(!getBypass()) {
            // TODO
            // 1. Fill buffer with data from WAV file
            // 2. Apply gain to each channel
            
            for (auto ch = 0; ch < 2; ++ch) {
                // Apply ramp to get best results if values are different
                auto old_gain = linearGainPrevious[ch];
                auto new_gain = linearGain[ch];
                if (old_gain != new_gain) {
                    buffer.applyGainRamp(ch, 0, buffer.getNumSamples(), old_gain, new_gain);
                    linearGainPrevious[ch] = new_gain;
                } else {
                    buffer.applyGain(ch, 0, buffer.getNumSamples(), new_gain);
                }
            }
        }
    }
    
    void TrackProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        // TODO
    }
    
    void TrackProcessor::releaseResources() {
        // TODO
    }
    
    void TrackProcessor::setFilePath(const juce::String &f) {
        if(filePath != f) {
            filePath = f;
            auto file = juce::File(filePath);
            // Swapping pointers should be thread-safe
            if (file.existsAsFile()){
                // Reader
                auto reader = formatManager.createReaderFor(file);
                
                // Transport
                currentAudioFileSource.reset(new AudioFormatReaderSource(reader, true)); // true means it takes pointer ownership
                transportSource.setSource(currentAudioFileSource.get());
            } else {
                currentAudioFileSource.reset(nullptr);
                transportSource.setSource(nullptr);
            }
        }
    }
    
    const juce::String &TrackProcessor::getFilePath() const {
        return filePath;
    }
    
    bool TrackProcessor::getMute() const {
        return mute;
    }
    
    void TrackProcessor::setMute(bool value) {
        mute = value;
    }
    
    bool TrackProcessor::getSolo() const {
        return solo;
    }
    
    void TrackProcessor::setSolo(bool value) {
        solo = value;
    }
    bool TrackProcessor::getBypass() const {
        return bypass;
    }
    
    void TrackProcessor::setBypass(bool value) {
        bypass = value;
    }
    
    juce::AudioFormatManager &TrackProcessor::getFormatManager() {
        return formatManager;
    }

    /*
     int readPos = 0;
     void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
     {
     if (reader != 0)
     {
     reader->read(&buffer, 0, buffer.getNumSamples(), readPos, true, true);
     readPos += buffer.getNumSamples();
     if (readPos >= reader->lengthInSamples) {
     readPos = 0;
     }
     }
     _block->process(buffer);
     }
     */
    
}
