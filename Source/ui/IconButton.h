#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../utils/ResourceHelper.h"

namespace audioplayer {
    class IconButton : public juce::Button {
    public:
        struct Theme {
            juce::Colour normal{0xff262a33};
            juce::Colour hover{0xff2a2f3b};
            juce::Colour pressed{0xff21242d};
            juce::Colour disabled{0xaa232730};
            juce::Colour normalChecked{0xffff6767};
            juce::Colour hoverChecked{0xffff7777};
            juce::Colour pressedChecked{0xfff04343};
            juce::Colour disabledChecked{0x33ff6767};
        };
        
        IconButton();
        ~IconButton();
        
        void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
        void resized() override;
        
        void setTheme(const Theme &);
        void setIcon(ResourceHelper::IconID);
        
    private:
        Theme theme;
        ResourceHelper::IconID iconID {ResourceHelper::IconID::None};
        juce::DrawableComposite icon;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconButton)
    };
} // namespace audioplayer
