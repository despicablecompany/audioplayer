#include "MasterTrackProcessor.h"

namespace audioplayer {
    MasterTrackProcessor::MasterTrackProcessor() {
    }
    
    MasterTrackProcessor::~MasterTrackProcessor() {
    }
    
    void MasterTrackProcessor::setGain(float g) {
        if (g != gain) {
            gain = g;
            updateLinearGain();
        }
    }
    
    float MasterTrackProcessor::getGain() const {
        return gain;
    }
    
    void MasterTrackProcessor::setPan(float p) {
        if (p != pan) {
            pan = p;
            updateLinearGain();
        }
    }
    
    float MasterTrackProcessor::getPan() const {
        return pan;
    }
    
    void MasterTrackProcessor::updateLinearGain() {
        // Compute linear values
        auto g = std::pow(10.f, gain / 20.f) * M_SQRT2;
        auto p = (pan + 1.f) * M_PI_4;
        
        // Compensate for left and right channels
        linearGain[0] = g * cos(p) ; // left
        linearGain[1] = g * sin(p); // right
    }
    
    void MasterTrackProcessor::process(juce::AudioBuffer<float> &buffer) {
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
    
    void MasterTrackProcessor::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        // TODO
    }
    
    void MasterTrackProcessor::releaseResources() {
        // TODO
    }
}
