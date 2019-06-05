#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

namespace audioplayer {
    class Slider : public juce::Slider {
    public:
        struct Theme {
            juce::Colour handleNormal{0xffededed};
            juce::Colour handleHover{0xffffffff};
            juce::Colour handlePressed{0xffe9e9e9};
            juce::Colour handleDisabled{0x90232730};
            juce::Colour trackNormal{0xff454C68};
            juce::Colour trackDisabled{0x90454C68};
            juce::Colour grooveNormal{0xff818A97};
            juce::Colour grooveDisabled{0x90818A97};
        };
        
        Slider();
        ~Slider();
        
        void resized() override;
        void paint(Graphics &) override;
        
        void setTheme(const Theme &);
        
    private:
        Theme theme;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Slider)
    };
} // namespace audioplayer
