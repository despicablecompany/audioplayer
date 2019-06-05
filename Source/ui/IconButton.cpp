#include "IconButton.h"

namespace audioplayer {
    IconButton::IconButton() : juce::Button("") {
        icon.setSize(16, 16);
        setClickingTogglesState(true);
        addAndMakeVisible(&icon);
        setSize(28, 28);
    }
    
    IconButton::~IconButton() {
        
    }
    
    void IconButton::paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
        const auto global_rect = getLocalBounds();
        
        // Background
        const auto enabled = isEnabled();
        const auto checked = getToggleState();
        if (!enabled) {
            g.setColour(checked ? theme.disabledChecked : theme.disabled);
        } else if (shouldDrawButtonAsDown){
            g.setColour(checked ? theme.pressedChecked : theme.pressed);
        } else if (shouldDrawButtonAsHighlighted) {
            g.setColour(checked ? theme.hoverChecked : theme.hover);
        } else {
            g.setColour(checked ? theme.normalChecked : theme.normal);
        }
        g.fillRect(global_rect);
        
        // Shadows
        g.setColour(juce::Colours::white.withAlpha(0.05f));
        g.drawLine(global_rect.getX(), global_rect.getY() + 0.5f, global_rect.getRight(), global_rect.getY() + 0.5f, 1);
        g.setColour(juce::Colours::black.withAlpha(0.05f));
        g.drawLine(global_rect.getX(), global_rect.getBottom() - 0.5f, global_rect.getRight(), global_rect.getBottom() - 0.5f, 1);
    }
    
    void IconButton::resized() {
        const auto global_bounds = getBounds();
        icon.setBounds((global_bounds.getWidth() - icon.getWidth()) / 2,
                       (global_bounds.getHeight() - icon.getHeight()) / 2,
                       icon.getWidth(), icon.getHeight());
    }
    
    void IconButton::setTheme(const Theme &t) {
        theme = t;
        repaint();
    }
    
    void IconButton::setIcon(ResourceHelper::IconID id) {
        if (id != iconID) {
            iconID = id;
            icon.deleteAllChildren();
            icon.addAndMakeVisible(ResourceHelper::getDrawable(iconID, 16, 16));
            icon.setSize(16, 16);
        }
    }
} // namespace audioplayer
