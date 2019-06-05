#include "MasterTrack.h"

namespace audioplayer {
    MasterTrack::MasterTrack() {
        setOpaque(true);
        
        // Name
        nameLabel.setFont(juce::Font("SF Pro Text", 13, juce::Font::FontStyleFlags::plain));
        nameLabel.setColour(juce::Label::textColourId, Colour(0xffffffff));
        nameLabel.setMinimumHorizontalScale(1.f);
        nameLabel.setText("Master", juce::NotificationType::dontSendNotification);
        addAndMakeVisible(nameLabel);
        
        // Volume
        volSlider.setLabel("Vol.");
        volSlider.setValueFormatter(new SliderWithLabel::GainValueFormatter());
        volSlider.setOnSliderValueChanged([this](double value){
            processor.setGain(value);
        });
        addAndMakeVisible(volSlider);
        
        // Panoramic
        panSlider.setLabel("Pan.");
        panSlider.setValueFormatter(new SliderWithLabel::PanValueFormatter());
        panSlider.setOnSliderValueChanged([this](double value){
            processor.setPan(value);
        });
        addAndMakeVisible(panSlider);
        
        // Global size
        auto default_size = getSizeHint();
        setSize(default_size.first, default_size.second);
    }
    
    MasterTrack::~MasterTrack() {
    }
    
    void MasterTrack::paint(Graphics &g) {
        // Background
        g.fillAll(Colour(0xff22252D));
        
        // Shadows
        g.setColour(Colours::white.withAlpha(0.05f));
        g.drawLine(0.f, 0.5f, getBounds().getWidth(), 0.5f, 1);
    }
    
    void MasterTrack::resized() {
        const auto padding = 8;
        const auto spacing = 32;
        
        // Name
        const auto name_w = 64;
        const auto name_x = 11;
        nameLabel.setBounds(name_x, (getHeight() - 13) / 2, name_w, 13);
        
        // Volume and Panoramic
        const auto slider_size_hint = panSlider.getSizeHint();
        const auto sliders_available_w = getWidth() - name_w - name_x;
        const auto slider_actual_w = std::min(slider_size_hint.first, (sliders_available_w - spacing - 2 * padding) / 2);
        const auto slider_y = (getHeight() - slider_size_hint.second) / 2;
        panSlider.setBounds(getWidth() - slider_actual_w, slider_y, slider_actual_w, slider_size_hint.second);
        volSlider.setBounds(getWidth() - slider_actual_w * 2 - spacing, slider_y, slider_actual_w, slider_size_hint.second);
        

    }
    
    std::pair<int, int> MasterTrack::getSizeHint() const {
        return std::make_pair(300, 40);
    }
    
    audioplayer::MasterTrackProcessor &MasterTrack::getProcessor() {
        return processor;
    }
} // namespace audioplayer
