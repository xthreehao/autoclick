#include "keyboard_monitor.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <linux/input.h>
#include <sys/ioctl.h>

#define BITS_PER_ULONG (sizeof(unsigned long) * 8)
#define ULONG_INDEX(bit) ((bit) / BITS_PER_ULONG)
#define ULONG_BIT(bit) (1UL << ((bit) % BITS_PER_ULONG))

KeyboardMonitor::KeyboardMonitor(QObject *parent) : QObject(parent) {}

KeyboardMonitor::~KeyboardMonitor() {
    stop();
}

bool KeyboardMonitor::start() {
    m_fd = findKeyboardDevice();
    if (m_fd < 0) {
        qWarning() << "KeyboardMonitor: no keyboard device found";
        return false;
    }
    m_notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated, this, &KeyboardMonitor::onReadyRead);
    qDebug() << "KeyboardMonitor: started";
    return true;
}

void KeyboardMonitor::stop() {
    delete m_notifier;
    m_notifier = nullptr;
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

int KeyboardMonitor::findKeyboardDevice() {
    for (int i = 0; i < 32; i++) {
        char path[64];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);

        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) continue;

        // Check device name
        char name[256] = {0};
        if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
            close(fd); continue;
        }

        // Skip non-keyboard devices
        if (strstr(name, "Power") || strstr(name, "Lid") ||
            strstr(name, "Sleep") || strstr(name, "Video")) {
            close(fd); continue;
        }

        // Check for EV_KEY capability
        unsigned long ev_bits[ULONG_INDEX(EV_MAX) + 1] = {0};
        ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits);
        if (!(ev_bits[ULONG_INDEX(EV_KEY)] & ULONG_BIT(EV_KEY))) {
            close(fd); continue;
        }

        // Check for keyboard keys
        unsigned long key_bits[ULONG_INDEX(KEY_MAX) + 1] = {0};
        ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits);
        if (key_bits[ULONG_INDEX(KEY_A)] & ULONG_BIT(KEY_A)) {
            qDebug() << "KeyboardMonitor: using" << path << "-" << name;
            return fd;
        }

        close(fd);
    }
    return -1;
}

void KeyboardMonitor::setTriggerKey(unsigned short keyCode) {
    m_triggerKey = keyCode;
}

void KeyboardMonitor::onReadyRead(int fd) {
    struct input_event ev;
    while (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
        if (ev.type == EV_KEY && ev.value == 1 && ev.code == m_triggerKey) {
            emit shortcutTriggered();
        }
    }
}
