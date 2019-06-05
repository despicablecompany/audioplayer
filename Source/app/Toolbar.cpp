#include "Toolbar.h"

#include <array>
#include <numeric>

namespace audioplayer {
    Toolbar::Toolbar() {
        setOpaque(true);
        
        // Rewind
        rewindButton.setBorderRadiusSide(ToolbarButton::BorderRadiusSide::Left);
        rewindButton.setIcon(ResourceHelper::IconID::Rewind);
        rewindButton.setTooltip("Go to start");
        addAndMakeVisible(rewindButton);
        rewindButton.setEnabled(false);
        
        // Play/Pause
        playPauseButton.setBorderRadiusSide(ToolbarButton::BorderRadiusSide::None);
        playPauseButton.setIcon(ResourceHelper::IconID::Play, 22, 22);
        playPauseButton.setIconChecked(ResourceHelper::IconID::Pause, 22, 22);
        playPauseButton.setClickingTogglesState(true);
        playPauseButton.setShowEdgeChecked(false);
        playPauseButton.setTooltip("Play/Pause");
        playPauseButton.onClick = [this]() {
          if (playPauseCallback)
              playPauseCallback(playPauseButton.getToggleState());
        };
        addAndMakeVisible(playPauseButton);
        
        // Stop
        stopButton.setBorderRadiusSide(ToolbarButton::BorderRadiusSide::None);
        stopButton.setIcon(ResourceHelper::IconID::Stop);
        stopButton.setTooltip("Stop");
        addAndMakeVisible(stopButton);
        
        // Forward
        forwardButton.setBorderRadiusSide(ToolbarButton::BorderRadiusSide::Right);
        forwardButton.setIcon(ResourceHelper::IconID::Forward);
        forwardButton.setTooltip("Go to end");
        addAndMakeVisible(forwardButton);
        
        // Loop
        loopButton.setBorderRadiusSide(ToolbarButton::BorderRadiusSide::All);
        loopButton.setIcon(ResourceHelper::IconID::Loop);
        loopButton.setTooltip("Toggle loop");
        loopButton.setClickingTogglesState(true);
        loopButton.onClick = [this]() {
            if (loopCallback)
                loopCallback(loopButton.getToggleState());
        };
        addAndMakeVisible(loopButton);
        
        // Global size
        auto default_size = getSizeHint();
        setSize(default_size.first, default_size.second);
    }
    
    Toolbar::~Toolbar() {
    }
    
    void Toolbar::paint(Graphics &g) {
        g.fillAll(Colour(0xff22252D));
    }
    
    void Toolbar::resized() {
        // Transport
        const auto small_spacing = 1;
        const auto big_spacing = 8;
        const auto button_size = rewindButton.getSizeHint(); // Consider all buttons have the same width
        auto transport_w = button_size.first * 5 + small_spacing * 3 + big_spacing;
        auto transport_x = (getWidth() - transport_w) / 2;
        auto transport_y = (getHeight() - button_size.second) / 2;

        rewindButton.setBounds(transport_x, transport_y, button_size.first, button_size.second);
        transport_x += button_size.first + small_spacing;
        
        playPauseButton.setBounds(transport_x, transport_y, button_size.first, button_size.second);
        transport_x += button_size.first + small_spacing;
        
        stopButton.setBounds(transport_x, transport_y, button_size.first, button_size.second);
        transport_x += button_size.first + small_spacing;
        
        forwardButton.setBounds(transport_x, transport_y, button_size.first, button_size.second);
        transport_x += button_size.first + big_spacing;
        
        loopButton.setBounds(transport_x, transport_y, button_size.first, button_size.second);
    }
    
    std::pair<int, int> Toolbar::getSizeHint() const {
        return std::make_pair(300, 40);
    }
    
    void Toolbar::setOnRewindClicked(const std::function<void()> &callback) {
        rewindButton.onClick = callback;
    }
    
    void Toolbar::setOnForwardClicked(const std::function<void()> &callback) {
        forwardButton.onClick = callback;
    }
    
    void Toolbar::setOnPlayPauseClicked(const std::function<void(bool)> &callback) {
        playPauseCallback = callback;
    }
    
    void Toolbar::setOnStopClicked(const std::function<void()> &callback) {
        stopButton.onClick = callback;
    }
    
    void Toolbar::setOnLoopClicked(const std::function<void(bool)> &callback) {
        loopCallback = callback;
    }
    
} // namespace audioplayer
