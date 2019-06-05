#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../ui/SliderWithLabel.h"
#include "../audio/MasterTrackProcessor.h"

#include <utility>
#include <functional>

namespace audioplayer {
    /*
     Class for the app MasterTrack.
     */
    class MasterTrack : public juce::Component {
    public:
        MasterTrack();
        ~MasterTrack();
        
    public:
        void paint(Graphics&) override;
        void resized() override;
        
        std::pair<int, int> getSizeHint() const;
        
        audioplayer::MasterTrackProcessor &getProcessor();

    private:
        juce::Label nameLabel;
        audioplayer::SliderWithLabel volSlider;
        audioplayer::SliderWithLabel panSlider;
        
        audioplayer::MasterTrackProcessor processor;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterTrack)
    };
} // namespace audioplayer
