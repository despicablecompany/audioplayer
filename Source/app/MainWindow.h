#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

namespace audioplayer {
/*
 This class implements the desktop window that contains an instance of
 our MainComponent class.
 */
class MainWindow : public juce::DocumentWindow {
public:
    MainWindow(juce::String name = "");
    ~MainWindow();
    
    void closeButtonPressed() override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
} // namespace audioplayer
