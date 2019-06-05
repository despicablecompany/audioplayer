#include "MainWindow.h"
#include "MainComponent.h"

namespace audioplayer {
MainWindow::MainWindow(String name)
    : DocumentWindow (name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons) {
    setUsingNativeTitleBar(true);
    setContentOwned(new audioplayer::MainComponent(), true);
    setResizable(true, true);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    setResizeLimits(400, 200, 1920 * 2, 1080 * 2);
}

MainWindow::~MainWindow() {
}

void MainWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}
} // namespace audioplayer
