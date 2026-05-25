#include <iostream>
#include <QApplication>
#include "head.h"
#include "keyboard_monitor.h"

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        Log log;
        log.logINFO("已成功启动");

        KeyboardMonitor rightMonitor;
        rightMonitor.setTriggerKey(KEY_R);
        if (!rightMonitor.start()) {
                qWarning("右键连点：键盘监控启动失败");
        }

        KeyboardMonitor leftMonitor;
        leftMonitor.setTriggerKey(KEY_TAB);
        if (!leftMonitor.start()) {
                qWarning("左键连点：键盘监控启动失败");
        }

        ui(&rightMonitor, &leftMonitor);
        return app.exec();
}