#include <iostream>
#include <QApplication>
#include "head.h"
#include "keyboard_monitor.h"

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        Log log;
        log.logINFO("已成功启动");

        KeyboardMonitor monitor;
        if (!monitor.start()) {
                qWarning("全局热键启动失败，无法读取键盘设备");
        }

        ui(&monitor);
        return app.exec();
}