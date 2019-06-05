#include "../JuceLibraryCode/JuceHeader.h"
#include "app/MainWindow.h"

class AudioPlayerApplication  : public JUCEApplication {
public:
    AudioPlayerApplication() {}
    ~AudioPlayerApplication() {}

    const String getApplicationName() override { return ProjectInfo::projectName; }
    const String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String& commandLine) override {
        mainWindow.reset(new audioplayer::MainWindow(getApplicationName()));
    }

    void shutdown() override {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override {
        quit();
    }

    void anotherInstanceStarted(const String& commandLine) override {
    }

private:
    std::unique_ptr<audioplayer::MainWindow> mainWindow;
    juce::SharedResourcePointer<juce::TooltipWindow> tooltipWindow;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (AudioPlayerApplication)
