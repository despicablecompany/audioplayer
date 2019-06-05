#include "Track.h"

namespace audioplayer {
    Track::Track() : thumbnail (512, processor.getFormatManager(), thumbnailCache) {
        // Name
        nameLabel.setFont(juce::Font("SF Pro Text", 13, juce::Font::FontStyleFlags::plain));
        nameLabel.setColour(juce::Label::textColourId, Colour(0xffffffff));
        nameLabel.setMinimumHorizontalScale(1.f);
        addAndMakeVisible(nameLabel);
        
        // Solo button
        soloButton.setIcon(ResourceHelper::IconID::Headphones);
        soloButton.setTheme(IconButton::Theme{
            juce::Colour{0xff262a33}, // normal
            juce::Colour{0xff2a2f3b}, // hover
            juce::Colour{0xff21242d}, // pressed
            juce::Colour{0xaa232730}, // disabled
            juce::Colour{0xffff6767}, // normalChecked
            juce::Colour{0xffff7777}, // hoverChecked
            juce::Colour{0xfff04343}, // pressedChecked
            juce::Colour{0x33ff6767}  // disabledChecked
        });
        soloButton.setTooltip("Solo");
        soloButton.onClick = [this](){
            //std::cout << "Solo track " << getTrackIndex() << std::endl;
            processor.setSolo(soloButton.getToggleState());
            if(soloMuteCallback)
                soloMuteCallback(soloButton.getToggleState(), muteButton.getToggleState());
        };
        addAndMakeVisible(soloButton);
        
        // Mute button
        muteButton.setIcon(ResourceHelper::IconID::Mute);
        muteButton.setTheme(IconButton::Theme{
            juce::Colour{0xff262a33}, // normal
            juce::Colour{0xff2a2f3b}, // hover
            juce::Colour{0xff21242d}, // pressed
            juce::Colour{0xaa232730}, // disabled
            juce::Colour{0xffECB33D}, // normalChecked
            juce::Colour{0xffFBC65A}, // hoverChecked
            juce::Colour{0xffD79A1E}, // pressedChecked
            juce::Colour{0x33ECB33D}  // disabledChecked
        });
        muteButton.setTooltip("Mute");
        muteButton.onClick = [this](){
            //std::cout << "Mute track " << getTrackIndex() << std::endl;
            processor.setMute(muteButton.getToggleState());
            if(soloMuteCallback)
                soloMuteCallback(soloButton.getToggleState(), muteButton.getToggleState());
        };
        addAndMakeVisible(muteButton);
        
        // Sliders
        volSlider.setLabel("Vol.");
        volSlider.setValueFormatter(new SliderWithLabel::GainValueFormatter());
        volSlider.setOnSliderValueChanged([this](double value){
            std::cout << "Gain track " << getTrackIndex() << " = " << value << std::endl;
            processor.setGain(value);
        });
        addAndMakeVisible(volSlider);
        
        panSlider.setLabel("Pan.");
        panSlider.setValueFormatter(new SliderWithLabel::PanValueFormatter());
        panSlider.setOnSliderValueChanged([this](double value){
            std::cout << "Pan track " << getTrackIndex() << " = " << value << std::endl;
            processor.setPan(value);
        });
        addAndMakeVisible(panSlider);
        
        // Thumbnail
        thumbnail.addChangeListener(this);
        
        // Global size
        setSize(300, 96);
    }
    
    Track::~Track() {
        setLookAndFeel(nullptr);
    }
    
    bool Track::isInterestedInFileDrag(const juce::StringArray &files) {
        for (const auto &f : files) {
            if (f.endsWithIgnoreCase(".wav"))
                return true;
        }
        return false;
    }
    
    void Track::filesDropped(const juce::StringArray &files, int x, int y) {
        if(!files.isEmpty()){
            setFilePath(files[0]);
        }
    }
    
    void Track::changeListenerCallback(ChangeBroadcaster* source) {
        if(source == &thumbnail) {
            repaint();
        }
    }
    
    void Track::updateNameLabel() {
        nameLabel.setText(juce::String(index + 1) + juce::String(". ") + name, juce::NotificationType::dontSendNotification);
    }
    
    void Track::setTrackIndex(unsigned int i) {
        if (i != index) {
            index = i;
            updateNameLabel();
        }
    }
    
    unsigned int Track::getTrackIndex() const {
        return index;
    }
    
    void Track::setTrackColour(const juce::Colour &c) {
        if (c != colour) {
            colour = c;
            repaint();
        }
    }
    
    const juce::Colour &Track::getTrackColour() const {
        return colour;
    }
    
    void Track::setTrackName(const juce::String &name) {
        if(name != this->name) {
            this->name = name;
            updateNameLabel();
        }
    }
    
    juce::String Track::getTrackName() const {
        return nameLabel.getText();
    }
    
    void Track::setFilePath(const juce::String &filePath) {
        if(filePath != processor.getFilePath()) {
            // Update processor
            processor.setFilePath(filePath);
            
            // Update thumbnail
            auto input_source = new juce::FileInputSource(filePath);
            thumbnail.setSource(input_source);
            repaint();
        }
    }
    
    const juce::String &Track::getFilePath() const {
        return processor.getFilePath();
    }
    
    void Track::paint(juce::Graphics& g) {
        g.saveState();
        {
            auto global_rect = getLocalBounds();
            global_rect.setX(0);
            global_rect.setY(0);
            global_rect.reduce(2, 0);
            
            // Clip
            Path clip_region;
            clip_region.addRoundedRectangle(global_rect, 4);
            g.reduceClipRegion(clip_region);
            
            // Global fill
            g.setColour({38, 42, 51});
            g.fillRect(global_rect);
            
            // Header
            Rectangle<int> colorRect(headerRect.getX(), headerRect.getY(), 8, headerRect.getHeight());
            g.setColour(colour);
            g.fillRect(colorRect);
            g.setColour(Colour(17, 19, 21).withAlpha(0.75f));
            g.drawLine(headerRect.getRight() - 0.5f, 0, headerRect.getRight() - 0.5f, headerRect.getBottom(), 1);
            
            // Lines in the header
            g.setColour({31, 35, 41});
            g.drawLine(headerRect.getX() + 8.5f, headerRect.getY() + 28.5f, headerRect.getRight() - 1, headerRect.getY() + 28.5f); // horizontal
            auto line_1_x = headerRect.getRight() - 1 - soloButton.getWidth() - 0.5f;
            g.drawLine(line_1_x, headerRect.getY(), line_1_x, headerRect.getY() + 28.5f); // vertical 1
            g.drawLine(line_1_x - 29, headerRect.getY(), line_1_x - 29, headerRect.getY() + 28.5f); // vertical 2
            
            // Content Background
            Rectangle<int> background_rect(headerRect.getRight(), 0, global_rect.getWidth() - headerRect.getWidth(), global_rect.getHeight());
            g.setColour({29, 32, 38});
            g.fillRect(background_rect);
            
            // Waveform
            auto waveform_color = getTrackColour();
            g.setColour(waveform_color);
            thumbnail.drawChannels(g // painter
                                   , background_rect.reduced(0, 8) // area
                                   , 0 // start
                                   , thumbnail.getTotalLength() // end
                                   , 1.0f //zoom
                                   );
            
            // Playhead
            // TODO
            
            // Shadows
            g.setColour(Colours::white.withAlpha(0.05f));
            g.drawLine(0.f, 0.5f, global_rect.getWidth(), 0.5f, 1);
            g.setColour(Colours::black.withAlpha(0.05f));
            g.drawLine(0.f, global_rect.getHeight() - 0.5f, global_rect.getWidth(), global_rect.getHeight() - 0.5f, 1);
        }
        g.restoreState();
    }
    
    void Track::resized() {
        auto global_rect = getLocalBounds();
        global_rect.setX(0);
        global_rect.setY(0);
        global_rect.reduce(2, 0);
        
        headerRect = Rectangle<int>(global_rect.getX(), global_rect.getY(), 220, global_rect.getHeight());
                
        // Header
        auto spacing = 1;
        Rectangle<int> header_top_rect(headerRect);
        header_top_rect.setHeight(28);
        header_top_rect.removeFromLeft(8 + spacing);
        header_top_rect.removeFromRight(1);
        
        soloButton.setBounds(header_top_rect.removeFromRight(soloButton.getWidth()));
        header_top_rect.removeFromRight(spacing);
        muteButton.setBounds(header_top_rect.removeFromRight(muteButton.getWidth()));
        header_top_rect.setHeight(13);
        header_top_rect.setY(header_top_rect.getY() + (28 - 13)  / 2 + 2);
        nameLabel.setBounds(header_top_rect);
        
        // Sliders
        Rectangle<int> header_bottom_rect(headerRect);
        auto padding = 4;
        header_bottom_rect.removeFromTop(28 + spacing + padding);
        header_bottom_rect.removeFromRight(padding + 1);
        header_bottom_rect.removeFromLeft(8 + spacing + padding);
        header_bottom_rect.removeFromBottom(padding);
        
        auto vol_rect = header_bottom_rect.removeFromTop(volSlider.getHeight());
        vol_rect.setHeight(std::max(vol_rect.getHeight(), volSlider.getHeight()));
        volSlider.setBounds(vol_rect);
        
        auto pan_rect = header_bottom_rect.removeFromTop(panSlider.getHeight());
        pan_rect.setHeight(std::max(pan_rect.getHeight(), panSlider.getHeight()));
        panSlider.setBounds(pan_rect);
    }
    
    audioplayer::TrackProcessor &Track::getProcessor() {
        return processor;
    }
    
    void Track::setOnSoloMuteChanged(const std::function<void(bool, bool)> &callback) {
        soloMuteCallback = callback;
    }
} // namespace audioplayer
