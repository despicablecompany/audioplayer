#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../ui/IconButton.h"
#include "../ui/SliderWithLabel.h"
#include "../audio/TrackProcessor.h"

#include <array>
#include <memory>
#include <functional>

namespace audioplayer {
    /*
     Class for a track.
     */
    class Track : public juce::Component,
                  public juce::FileDragAndDropTarget,
                  public juce::DragAndDropContainer,
                  public juce::ChangeListener {
    public:
        Track();
        ~Track();
        
        void setTrackIndex(unsigned int);
        unsigned int getTrackIndex() const;
        
        void setTrackColour(const juce::Colour &);
        const juce::Colour &getTrackColour() const;
        
        void setTrackName(const juce::String &);
        juce::String getTrackName() const;
        
        void setFilePath(const juce::String &filePath);
        const juce::String &getFilePath() const;
        
        audioplayer::TrackProcessor &getProcessor();
        
        void setOnSoloMuteChanged(const std::function<void(bool, bool)> &);
        
        static juce::Colour generateColour();
    
    public:
        void paint(Graphics&) override;
        void resized() override;
        
        bool isInterestedInFileDrag(const juce::StringArray &files) override;
        void filesDropped(const juce::StringArray &files, int x, int y) override;
        
        void changeListenerCallback (ChangeBroadcaster*) override;
        
    private:
        void updateNameLabel();
        
    private:
        audioplayer::TrackProcessor processor;
        int index {0};
        juce::String name{"No name"};
        juce::Colour colour {107, 74, 194};
        Rectangle<int> headerRect;
        
        juce::Label nameLabel;
        audioplayer::IconButton soloButton;
        audioplayer::IconButton muteButton;
        audioplayer::SliderWithLabel volSlider;
        audioplayer::SliderWithLabel panSlider;
        std::function<void(bool, bool)> soloMuteCallback;
        juce::AudioThumbnailCache thumbnailCache{1};
        juce::AudioThumbnail thumbnail;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
    };
} // namespace audioplayer
