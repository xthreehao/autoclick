#include "head.h"
#include "keyboard_monitor.h"

void ui(KeyboardMonitor *rightMonitor, KeyboardMonitor *leftMonitor)
{
        MainWindow *mainWindow = new MainWindow;
        mainWindow->setMonitorright(rightMonitor);
        mainWindow->setMonitorleft(leftMonitor);
        mainWindow->show();
}
