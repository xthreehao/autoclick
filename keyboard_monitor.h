#pragma once
#include <QObject>
#include <QSocketNotifier>
#include <linux/input.h>

class KeyboardMonitor : public QObject {
    Q_OBJECT
public:
    explicit KeyboardMonitor(QObject *parent = nullptr);
    ~KeyboardMonitor();
    bool start();
    void stop();
    void setTriggerKey(unsigned short keyCode);

signals:
    void shortcutTriggered();

private slots:
    void onReadyRead(int fd);

private:
    int findKeyboardDevice();
    int m_fd = -1;
    QSocketNotifier *m_notifier = nullptr;
    unsigned short m_triggerKey = KEY_R;
};
