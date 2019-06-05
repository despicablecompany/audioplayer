#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../ui/ToolbarButton.h"
#include "../ui/SliderWithLabel.h"

#include <utility>
#include <functional>

namespace audioplayer {
    /*
     Class for the app Toolbar.
     */
    class Toolbar : public juce::Component {
    public:
        Toolbar();
        ~Toolbar();
        
    public:
        void paint(Graphics&) override;
        void resized() override;
        
        std::pair<int, int> getSizeHint() const;
        
        void setOnRewindClicked(const std::function<void()> &);
        void setOnForwardClicked(const std::function<void()> &);
        void setOnPlayPauseClicked(const std::function<void(bool)> &);
        void setOnStopClicked(const std::function<void()> &);
        void setOnLoopClicked(const std::function<void(bool)> &);
        
    private:
        audioplayer::ToolbarButton rewindButton;
        audioplayer::ToolbarButton forwardButton;
        audioplayer::ToolbarButton playPauseButton;
        std::function<void(bool)> playPauseCallback; // wrapped inside default callback
        audioplayer::ToolbarButton stopButton;
        audioplayer::ToolbarButton loopButton;
        std::function<void(bool)> loopCallback; // wrapped inside default callback
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Toolbar)
    };
} // namespace audioplayer
