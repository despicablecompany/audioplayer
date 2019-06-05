#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "Track.h"
#include "Toolbar.h"
#include "MasterTrack.h"

namespace audioplayer {
    /*
     Main window component.
     */
    class MainComponent : public juce::AudioAppComponent,
                          public juce::FileDragAndDropTarget,
                          public juce::DragAndDropContainer {
    public:
        MainComponent();
        ~MainComponent();
        
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;
        
        void paint(juce::Graphics& g) override;
        void resized() override;
        
        bool isInterestedInFileDrag(const juce::StringArray &files) override;
        void filesDropped(const juce::StringArray &files, int x, int y) override;
                              
    private:
        void updateAllTracksBypass();
        
    private:
        std::vector<std::shared_ptr<audioplayer::Track>> tracks;
        audioplayer::Toolbar toolbar;
        audioplayer::MasterTrack masterTrack;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
    };
} // namespace audioplayer
