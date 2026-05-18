#include "head.h"
#include <QThread>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

bool AutoClickRunning = false;

static int uinput_fd = -1;

bool setup_uinput(){
        uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if(uinput_fd < 0) return false;

        ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
        ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);

        struct uinput_setup usetup;
        memset(&usetup, 0, sizeof(usetup));
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor = 1;
        usetup.id.product = 1;
        strcpy(usetup.name, "AutoClick Mouse");

        ioctl(uinput_fd, UI_DEV_SETUP, &usetup);
        ioctl(uinput_fd, UI_DEV_CREATE);
        return true;
}

void cleanup_uinput(){
        if(uinput_fd >= 0){
                ioctl(uinput_fd, UI_DEV_DESTROY);
                close(uinput_fd);
                uinput_fd = -1;
        }
}

void Autoclick(int cps){
        if(!AutoClickRunning) return;
        if(!setup_uinput()) return;

        int delayMs = 1000 / cps;

        while(AutoClickRunning){
#ifdef Q_OS_WIN
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#else
                struct input_event ev;
                memset(&ev, 0, sizeof(ev));

                ev.type = EV_KEY;
                ev.code = BTN_LEFT;
                ev.value = 1;
                write(uinput_fd, &ev, sizeof(ev));

                ev.type = EV_SYN;
                ev.code = SYN_REPORT;
                ev.value = 0;
                write(uinput_fd, &ev, sizeof(ev));

                memset(&ev, 0, sizeof(ev));
                ev.type = EV_KEY;
                ev.code = BTN_LEFT;
                ev.value = 0;
                write(uinput_fd, &ev, sizeof(ev));

                ev.type = EV_SYN;
                ev.code = SYN_REPORT;
                ev.value = 0;
                write(uinput_fd, &ev, sizeof(ev));
#endif
                QThread::msleep(delayMs);
        }

        cleanup_uinput();
}
