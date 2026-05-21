#pragma once
#include <QObject>
#include <QSocketNotifier>

class KeyboardMonitor : public QObject {
    Q_OBJECT
public:
    explicit KeyboardMonitor(QObject *parent = nullptr);
    ~KeyboardMonitor();
    bool start();
    void stop();

signals:
    void shortcutTriggered();

private slots:
    void onReadyRead(int fd);

private:
    int findKeyboardDevice();
    int m_fd = -1;
    QSocketNotifier *m_notifier = nullptr;
};
