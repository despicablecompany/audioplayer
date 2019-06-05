#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../utils/ResourceHelper.h"

#include <utility>

namespace audioplayer {
    class ToolbarButton : public juce::Button {
    public:
        struct Theme {
            juce::Colour normal{0xff333748};
            juce::Colour hover{0xff3E4358};
            juce::Colour pressed{0xff2D303F};
            juce::Colour disabled{0x90333748};
            juce::Colour edgeChecked{0xff22BD82};
        };
        
        enum class BorderRadiusSide {
            None,
            Left,
            Right,
            All
        };
        
    public:
        ToolbarButton();
        ~ToolbarButton();
        
        void setTheme(const Theme &);
        void setIcon(ResourceHelper::IconID, int width = 16, int height = 16);
        void setIconChecked(ResourceHelper::IconID, int width = 16, int height = 16);
        void setBorderRadius(unsigned int);
        void setBorderRadiusSide(BorderRadiusSide);
        void setShowEdgeChecked(bool);
        std::pair<int, int> getSizeHint() const;
        
    public:
        void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
        void resized() override;
        void enablementChanged() override;
        void clicked() override;
        
    private:
        Theme theme;
        ResourceHelper::IconID iconID {ResourceHelper::IconID::None};
        ResourceHelper::IconID iconIDChecked {ResourceHelper::IconID::None};
        juce::DrawableComposite icon;
        juce::DrawableComposite iconChecked;
        unsigned int borderRadius {2};
        BorderRadiusSide borderRadiusSide {BorderRadiusSide::All};
        bool showEdgeChecked {true};
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolbarButton)
    };
} // namespace audioplayer
