#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "Slider.h"

#include <functional>
#include <utility>

namespace audioplayer {
    /*
     Component containing a slider with its labels.
     */
    class SliderWithLabel : public juce::Component, public juce::Slider::Listener {
    public:
        class ValueFormatter {
        public:
            ValueFormatter() {}
            virtual ~ValueFormatter() {}
            
            virtual juce::String getTextForValue(double value) = 0;
            virtual double getCenter() = 0;
        };
        
        class GainValueFormatter : public ValueFormatter {
        public:
            GainValueFormatter() {}
            ~GainValueFormatter() {}
            juce::String getTextForValue(double value) override;
            double getCenter() override;
        };
        
        class PanValueFormatter : public ValueFormatter {
        public:
            PanValueFormatter() {}
            ~PanValueFormatter() {}
            juce::String getTextForValue(double value) override;
            double getCenter() override;
        };
        
    public:
        SliderWithLabel();
        ~SliderWithLabel();
        
        void setLabel(const juce::String &);
        void setOnSliderValueChanged(const std::function<void(double)> &callback);
        double sliderValue() const;
        std::pair<int, int> getSizeHint() const;
        void setValueFormatter(ValueFormatter *formatter);
        
    public:
        void resized() override;
        void sliderValueChanged(juce::Slider *slider) override;
        
    private:
        void updateValueLabel();
        
    private:
        juce::Label label;
        juce::Label valueLabel;
        audioplayer::Slider slider;
        std::function<void(double)> sliderCallback;
        std::unique_ptr<ValueFormatter> formatter;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderWithLabel)
    };
} // namespace audioplayer
