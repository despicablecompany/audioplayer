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
        
        // Display info for drag n drop if no tracks
        if(tracks.size() == 0) {
            auto text_rect_w = std::min(getWidth() - 16, 400);
            auto text_rect_h = std::min(getHeight() - 16 - toolbar.getSizeHint().second - masterTrack.getSizeHint().second, 200);
            Rectangle<int> text_rect((getWidth() - text_rect_w) / 2, (getHeight() - text_rect_h) / 2, text_rect_w, text_rect_h);
            g.setFont(juce::Font("SF Pro Text", 17, juce::Font::FontStyleFlags::plain));
            g.setColour(juce::Colour(0xff818A97));
            g.drawText("Drag and drop files here to open them as tracks.", text_rect, juce::Justification::centred);
            
            g.setColour(juce::Colour(0x70818A97));
            juce::Path path;
            auto stroke_thickness = 1.0;
            path.addRoundedRectangle(text_rect.getX() - stroke_thickness / 2, text_rect.getY() - stroke_thickness / 2, text_rect.getWidth()  + stroke_thickness, text_rect.getHeight() + stroke_thickness, 8);
            juce::PathStrokeType stroke_type(stroke_thickness, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded);
            float dash_length[2];
            dash_length[0] = 4;
            dash_length[1] = 8;
            stroke_type.createDashedStroke(path, path, dash_length, 2);
            g.strokePath(path, stroke_type);
        }
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
        if(!files.isEmpty()){
            for (const auto &s : files) {
                if (s.endsWithIgnoreCase(".wav"))
                    addTrack(s);
            }
        }
    }
    
    void MainComponent::addTrack(const juce::String &filePath) {
        auto track = new Track();
        auto track_index = tracks.size();
        track->setTrackIndex(track_index);
        auto track_name = filePath.isEmpty() ? juce::String("Track ") + juce::String(track_index + 1) : juce::File(filePath).getFileNameWithoutExtension();
        track->setTrackName(track_name);
        track->setTrackColour(Track::generateColour());
        track->setFilePath(filePath);
        track->setOnSoloMuteChanged([this](bool, bool){
            updateAllTracksBypass();
        });
        tracks.add(track);
        addAndMakeVisible(track);
        resized();
    }
} // namespace audioplayer
