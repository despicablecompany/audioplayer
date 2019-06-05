#include "ToolbarButton.h"

namespace audioplayer {
    ToolbarButton::ToolbarButton() : juce::Button("") {
        icon.setSize(16, 16);
        iconChecked.setSize(16, 16);
        addAndMakeVisible(icon);
        addAndMakeVisible(iconChecked);
        iconChecked.setVisible(false);
        const auto default_size = getSizeHint();
        setSize(default_size.first, default_size.second);
    }
    
    ToolbarButton::~ToolbarButton() {
        
    }
    
    void ToolbarButton::paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
        const auto global_rect = getLocalBounds();
        g.saveState();
        {
            // Clip, depending on the border radius
            if(borderRadiusSide != BorderRadiusSide::None && borderRadius != 0) {
                Path clip_region;
                juce::Rectangle<int> left_rect(global_rect.getX(), global_rect.getY(), global_rect.getWidth() / 2 + 1, global_rect.getHeight());
                juce::Rectangle<int> right_rect(global_rect.getX() + global_rect.getWidth() / 2 - 1, global_rect.getY(), global_rect.getWidth() / 2 + 1, global_rect.getHeight());
                switch (borderRadiusSide) {
                    case BorderRadiusSide::Left:
                        clip_region.addRoundedRectangle(left_rect, borderRadius);
                        clip_region.addRectangle(right_rect);
                        break;
                    case BorderRadiusSide::Right:
                        clip_region.addRoundedRectangle(right_rect, borderRadius);
                        clip_region.addRectangle(left_rect);
                        break;
                    case BorderRadiusSide::All:
                        clip_region.addRoundedRectangle(left_rect, borderRadius);
                        clip_region.addRoundedRectangle(right_rect, borderRadius);
                    default:
                        break;
                }
                g.reduceClipRegion(clip_region);
            }
            
            // Background
            const auto enabled = isEnabled();
            if (!enabled) {
                g.setColour(theme.disabled);
            } else if (shouldDrawButtonAsDown){
                g.setColour(theme.pressed);
            } else if (shouldDrawButtonAsHighlighted) {
                g.setColour(theme.hover);
            } else {
                g.setColour(theme.normal);
            }
            g.fillRect(global_rect);
            
            if (getToggleState() && showEdgeChecked) {
                g.setColour(theme.edgeChecked);
                g.fillRect(juce::Rectangle<int>(global_rect.getX(), global_rect.getBottom() - 3, global_rect.getWidth(), 3));
            }
            
            // Shadows
            if (enabled) {
                g.setColour(juce::Colours::white.withAlpha(0.05f));
                g.drawLine(global_rect.getX(), global_rect.getY() + 0.5f, global_rect.getRight(), global_rect.getY() + 0.5f, 1);
                g.setColour(juce::Colours::black.withAlpha(0.05f));
                g.drawLine(global_rect.getX(), global_rect.getBottom() - 0.5f, global_rect.getRight(), global_rect.getBottom() - 0.5f, 1);
            }
        }
        g.restoreState();
    }
    
    void ToolbarButton::resized() {
        const auto global_bounds = getBounds();
        icon.setBounds((global_bounds.getWidth() - icon.getWidth()) / 2,
                       (global_bounds.getHeight() - icon.getHeight()) / 2,
                       icon.getWidth(), icon.getHeight());
        iconChecked.setBounds((global_bounds.getWidth() - iconChecked.getWidth()) / 2,
                              (global_bounds.getHeight() - iconChecked.getHeight()) / 2,
                              iconChecked.getWidth(), iconChecked.getHeight());
    }
    
    void ToolbarButton::enablementChanged() {
        juce::Button::enablementChanged();
        auto opacity = isEnabled() ? 1.0f : 0.1f;
        icon.setAlpha(opacity);
        iconChecked.setAlpha(opacity);
    }
    
    void ToolbarButton::clicked() {
        juce::Button::clicked();
        auto checked = getToggleState();
        icon.setVisible(!checked);
        iconChecked.setVisible(checked);
        resized();
    }
    
    void ToolbarButton::setTheme(const Theme &t) {
        theme = t;
        repaint();
    }
    
    void ToolbarButton::setIcon(ResourceHelper::IconID id, int width, int height) {
        if (id != iconID) {
            iconID = id;
            icon.deleteAllChildren();
            icon.addAndMakeVisible(ResourceHelper::getDrawable(iconID, width, height));
            resized();
            
            // Same icon for checked state by default
            if (iconIDChecked == ResourceHelper::IconID::None) {
                setIconChecked(iconID, width, height);
            }
        }
    }
    
    void ToolbarButton::setIconChecked(ResourceHelper::IconID id, int width, int height) {
        if (id != iconIDChecked) {
            iconIDChecked = id;
            iconChecked.deleteAllChildren();
            iconChecked.addAndMakeVisible(ResourceHelper::getDrawable(iconIDChecked, width, height));
            resized();
        }
    }
    
    void ToolbarButton::setBorderRadius(unsigned int value) {
        if(value != borderRadius) {
            borderRadius = value;
            repaint();
        }
    }
    
    void ToolbarButton::setBorderRadiusSide(BorderRadiusSide side) {
        if(side != borderRadiusSide) {
            borderRadiusSide = side;
            repaint();
        }
    }
    
    void ToolbarButton::setShowEdgeChecked(bool value) {
        if(showEdgeChecked != value) {
            showEdgeChecked = value;
            repaint();
        }
    }
    
    std::pair<int, int> ToolbarButton::getSizeHint() const {
        return std::make_pair(28, 28);
    }

} // namespace audioplayer
