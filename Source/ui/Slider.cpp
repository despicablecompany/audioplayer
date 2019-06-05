#include "Slider.h"

namespace audioplayer {
    Slider::Slider() : juce::Slider(juce::Slider::SliderStyle::LinearBar, juce::Slider::NoTextBox) {
        setSize(100, 22);
        setRange(0.0, 1.0, 0.0);
        setValue(0.5);
        setDoubleClickReturnValue(true, 0.5);
    }
    
    Slider::~Slider() {
        
    }
    
    void Slider::resized() {
        juce::Slider::resized();
    }
    
    void Slider::paint(Graphics &g) {
        auto global_rect = getLocalBounds();
        
        auto current_value = (double)getValue();
        auto range = getRange();
        auto range_length = range.getLength();
        auto ratio = range_length > 0 ? (current_value - range.getStart()) / range_length : 0;
        const auto enabled = isEnabled();
        
        const auto handle_diameter = 16;
        Rectangle<int> track_rect(handle_diameter / 2, (global_rect.getHeight() - 4) / 2, global_rect.getWidth() - handle_diameter, 4);
        
        // Clip to draw the track and the groove
        g.saveState();
        {
            Path clip_region;
            clip_region.addRoundedRectangle(track_rect, 2);
            g.reduceClipRegion(clip_region);
            g.setColour(enabled ? theme.trackNormal : theme.trackDisabled);
            g.fillRect(track_rect);
            
            Rectangle<int> groove_rect(track_rect);
            groove_rect.setWidth(groove_rect.getWidth() * ratio);
            g.setColour(enabled ? theme.grooveNormal : theme.grooveDisabled);
            g.fillRect(groove_rect);
        }
        g.restoreState();
        
        // Draw the handle
        juce::DropShadow handle_shadow;
        handle_shadow.colour = isMouseButtonDown() ? juce::Colour(0x40000000) : juce::Colour(0x90000000);
        handle_shadow.radius = isMouseButtonDown() ? 1 : 2;
        handle_shadow.offset = juce::Point<int>(0, 1);
        juce::Path handle_path;
        handle_path.addEllipse(track_rect.getX() + ratio * track_rect.getWidth() - handle_diameter / 2, (global_rect.getHeight() - handle_diameter) / 2, handle_diameter, handle_diameter);
        handle_shadow.drawForPath(g, handle_path);
        
        auto handle_colour = isEnabled() ? (isMouseButtonDown() ? theme.handlePressed : (isMouseOver() ? theme.handleHover : theme.handleNormal)) : theme.handleDisabled;
        g.setColour(handle_colour);
        g.fillPath(handle_path);
    }
    
    void Slider::setTheme(const Theme &t) {
        theme = t;
        repaint();
    }
    
} // namespace audioplayer
