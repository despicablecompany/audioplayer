#pragma once

namespace juce {
    class Drawable;
} // namespace juce

namespace audioplayer {
    class ResourceHelper {
    private:
        ResourceHelper() = delete;
        
    public:
        // Enum to help identifying the icons in the inon bank.
        enum IconID {
            None,
            Forward,
            Headphones,
            Loop,
            Mute,
            Pause,
            Play,
            Rewind,
            Stop,
        };
        
        static const char *getBinaryData(IconID);
        static juce::Drawable *getDrawable(IconID, int width = 16, int height = 16);
    };
} // namespace audioplayer
