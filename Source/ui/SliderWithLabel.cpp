#include "SliderWithLabel.h"

namespace audioplayer {
    juce::String SliderWithLabel::GainValueFormatter::getTextForValue(double value) {
        if (value == 0) {
            return "-" + juce::String(juce::CharPointer_UTF8("\u221e")) + "dB";
        } else if (std::abs(value - getCenter()) < 0.01)  {
            return "0dB";
        } else {
            const auto knee = 7;
            const auto min = -60; // dB
            const auto max = 12; // dB
            const auto nonlinear_value =  1 - (1 - value) / std::powf(1 - std::powf(1 - value, knee), 1 / knee);
            auto denormalized_value = nonlinear_value * (max - min) + min;
            return (denormalized_value > 0 ? juce::String("+") : juce::String("")) + juce::String(denormalized_value, 1) + "dB";
        }
    }
    
    double SliderWithLabel::GainValueFormatter::getCenter() {
        // Resolve equation 0 = (1 - (1 - value) / std::powf(1 - std::powf(1 - value, knee), 1 / knee)) * (max - min) + min
        return 0.833337500178641;
    }
        
    juce::String SliderWithLabel::PanValueFormatter::getTextForValue(double value) {
        const auto denormalized_value = std::abs(std::floor(value * 200 - 100));
        if (denormalized_value == 0)
            return "Center";
        else
            return (value < 0.5 ? juce::String("L") : juce::String("R")) + juce::String(denormalized_value);
    }
    
    double SliderWithLabel::PanValueFormatter::getCenter() {
        // Pan is by default centered on 0.5 since it's linear
        return 0.5;
    }
    
    SliderWithLabel::SliderWithLabel() {
        // Label
        label.setText("Vol.", NotificationType::dontSendNotification);
        label.setFont(juce::Font("SF Pro Text", 13, juce::Font::FontStyleFlags::plain));
        label.setColour(juce::Label::textColourId, juce::Colour(0xff818A97));
        label.setMinimumHorizontalScale(1.f);
        addAndMakeVisible(label);
        
        // Label for the value
        valueLabel.setJustificationType(juce::Justification(juce::Justification::centred));
        valueLabel.setFont(juce::Font("SF Pro Text", 11, juce::Font::FontStyleFlags::plain));
        valueLabel.setColour(juce::Label::textColourId, juce::Colour(0xff818A97));
        valueLabel.setMinimumHorizontalScale(1.f);
        addAndMakeVisible(valueLabel);
        
        // Horizontal slider
        slider.addListener(this);
        addAndMakeVisible(slider);
        
        // Default size
        const auto default_size = getSizeHint();
        setSize(default_size.first, default_size.second);
    }
    
    SliderWithLabel::~SliderWithLabel() {
        slider.removeListener(this);
        setLookAndFeel(nullptr);
    }
    
    std::pair<int, int> SliderWithLabel::getSizeHint() const {
        return std::make_pair(300, 22);
    }

    void SliderWithLabel::resized() {
        auto global_bounds = getLocalBounds();
        auto old_h = global_bounds.getHeight();
        global_bounds.setHeight(std::max(global_bounds.getHeight(), slider.getHeight()));
        global_bounds.setY((global_bounds.getHeight() - old_h) / 2);
        
        label.setBounds(global_bounds.removeFromLeft(34));
        valueLabel.setBounds(global_bounds.removeFromRight(48));
        slider.setBounds(global_bounds);
    }
    
    void SliderWithLabel::setLabel(const juce::String &text) {
        label.setText(text, juce::NotificationType::dontSendNotification);
    }
    
    void SliderWithLabel::setOnSliderValueChanged(const std::function<void(double)> &callback) {
        sliderCallback = callback;
    }
    
    double SliderWithLabel::sliderValue() const {
        return slider.getValue();
    }
    
    void SliderWithLabel::sliderValueChanged(juce::Slider *s) {
        if(&slider == s) {
            // Call callback
            if(sliderCallback)
                sliderCallback(sliderValue());
            
            // Update label
            updateValueLabel();
        }
    }
    
    void SliderWithLabel::updateValueLabel() {
        if (formatter) {
            // Use formatter
            valueLabel.setText(formatter->getTextForValue(slider.getValue()), juce::NotificationType::dontSendNotification);
        } else {
            // Default: just display the value as is
            valueLabel.setText(juce::String(slider.getValue()), juce::NotificationType::dontSendNotification);
        }
    }

    void SliderWithLabel::setValueFormatter(ValueFormatter *f) {
        formatter.reset(f);
        if(formatter) {
            auto center = formatter->getCenter();
            slider.setDoubleClickReturnValue(true, center);
            slider.setValue(center);
        } else {
            slider.setDoubleClickReturnValue(true, 0.5);
            slider.setValue(0.5);
        }
        updateValueLabel();
    }

} // namespace audioplayer
