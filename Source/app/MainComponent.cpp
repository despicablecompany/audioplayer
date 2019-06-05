#include "MainComponent.h"

#include <array>
#include <tuple>
#include <algorithm>

namespace audioplayer {
    MainComponent::MainComponent() {
        setOpaque(true);
        
        // Toolbar
        toolbar.setOnRewindClicked([](){
            // TODO synchronize transport for each track
            std::cout << "Rewind" << std::endl;
        });
        toolbar.setOnForwardClicked([](){
            // TODO synchronize transport for each track
            std::cout << "Forward" << std::endl;
        });
        toolbar.setOnPlayPauseClicked([](bool shouldPlay){
            // TODO synchronize transport for each track
            if(shouldPlay)
                std::cout << "Play" << std::endl;
            else
                std::cout << "Pause" << std::endl;
        });
        toolbar.setOnStopClicked([](){
            // TODO synchronize transport for each track
            std::cout << "Stop" << std::endl;
        });
        toolbar.setOnLoopClicked([](bool shouldLoop){
            // TODO synchronize transport for each track
            std::cout << "Loop=" << shouldLoop << std::endl;
        });
        toolbar.setAlwaysOnTop(true);
        addAndMakeVisible(toolbar);
        
        // Master Track
        masterTrack.setAlwaysOnTop(true);
        addAndMakeVisible(masterTrack);
        
        // Tracks
        std::array<std::tuple<juce::String, juce::Colour, juce::String>, 4> track_infos {{
            std::make_tuple("Voice", juce::Colour(0xff22bd82), ""),
            std::make_tuple("Other", juce::Colour(0xff009eff), ""),
            std::make_tuple("Bass",  juce::Colour(0xff6b4ac2), ""),
            std::make_tuple("Drums", juce::Colour(0xffba4fae), ""),
        }};
        
        auto index = 0;
        for(const auto &track_info : track_infos) {
            auto track = std::make_shared<Track>();
            track->setTrackIndex(index++);
            track->setTrackName(std::get<0>(track_info));
            track->setTrackColour(std::get<1>(track_info));
            track->setFilePath(std::get<2>(track_info));
            track->setOnSoloMuteChanged([this](bool, bool){
                updateAllTracksBypass();
            });
            tracks.push_back(track);
            addAndMakeVisible(track.get());
        }
        
        // Global size
        setSize (800, 600);
        
        
        // Number of input and output channels that we want to open.
        setAudioChannels(0, 2);
    }
    
    MainComponent::~MainComponent() {
        setLookAndFeel(nullptr);
        shutdownAudio();
    }
    
    void MainComponent::updateAllTracksBypass() {
        // Check if any track is Solo
        auto any_solo_track = std::any_of(tracks.begin(), tracks.end(), [](const auto & t){
            return t->getProcessor().getSolo();
        });
        // If so, only play tracks that are Solo and not Mute
        for(auto &track : tracks) {
            auto &processor = track->getProcessor();
            auto should_play = any_solo_track ? processor.getSolo() : !processor.getMute();
            track->getProcessor().setBypass(!should_play);
        }
    }
    
    void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
        // Tracks
        for(auto &track : tracks) {
            track->getProcessor().prepareToPlay(samplesPerBlockExpected, sampleRate);
        }
        
        // Master
        masterTrack.getProcessor().prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    
    void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
        bufferToFill.clearActiveBufferRegion();
        
        // Tracks
        for (auto &track : tracks) {
            track->getProcessor().process(*bufferToFill.buffer);
        }
        
        // Master
        masterTrack.getProcessor().process(*bufferToFill.buffer);
    }
    
    void MainComponent::releaseResources() {
        // Tracks
        for(auto &track : tracks) {
            track->getProcessor().releaseResources();
        }
        
        // Master
        masterTrack.getProcessor().releaseResources();
    }
    
    void MainComponent::paint(Graphics& g) {
        g.fillAll(Colour(17, 19, 21));
    }
    
    void MainComponent::resized() {
        auto y = 0;
        auto spacing = 1;
        auto track_spacing = 2;
        auto total_width = getWidth();
        
        // Toolbar
        const auto toolbar_size_hint = toolbar.getSizeHint();
        toolbar.setBounds(0, 0, total_width, toolbar_size_hint.second);
        y += toolbar.getHeight() + spacing;
        
        // Master track
        const auto mastertrack_size_hint = masterTrack.getSizeHint();
        masterTrack.setBounds(0, getHeight() - mastertrack_size_hint.second, total_width, mastertrack_size_hint.second);
        
        // Tracks
        for(auto track : tracks) {
            auto track_height = track->getHeight();
            track->setBounds(0, y, total_width, track_height);
            y += track_height + track_spacing;
        }
        
        // Z-order
        masterTrack.toFront(false);
        toolbar.toFront(false);
    }

    bool MainComponent::isInterestedInFileDrag(const juce::StringArray &files) {
        for (const auto &f : files) {
            if (f.endsWithIgnoreCase(".wav"))
                return true;
        }
        return false;
    }
    
    void MainComponent::filesDropped(const juce::StringArray &files, int x, int y) {
        // TODO Add a track for the file
    }
} // namespace audioplayer
