#include "ResourceHelper.h"

#include "../../JuceLibraryCode/JuceHeader.h"
#include <memory>

namespace audioplayer {
    const char * ResourceHelper::getBinaryData(ResourceHelper::IconID id) {
        switch(id) {
            case Headphones:
                return BinaryData::headphones_svg;
            case Mute:
                return BinaryData::mute_svg;
            case Play:
                return BinaryData::play_svg;
            case Pause:
                return BinaryData::pause_svg;
            case Stop:
                return BinaryData::stop_svg;
            case Rewind:
                return BinaryData::rewind_svg;
            case Forward:
                return BinaryData::forward_svg;
            case Loop:
                return BinaryData::loop_svg;
            default:
                return nullptr;
        }
    }
    
    juce::Drawable *ResourceHelper::getDrawable(ResourceHelper::IconID iconID, int width, int height) {
        auto binary_data = ResourceHelper::getBinaryData(iconID);
        if (binary_data) {
            std::unique_ptr<juce::XmlElement> icon_svg_xml(juce::XmlDocument::parse(binary_data));
            auto drawable_svg = Drawable::createFromSVG(*(icon_svg_xml.get()));
            drawable_svg->setBounds(0, 0, width, height);
            return drawable_svg;
        }
        return nullptr;
    }
} // namespace audioplayer
