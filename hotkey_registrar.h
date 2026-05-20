#pragma once
#include <QObject>
#include <QString>
#include <QDBusPendingCallWatcher>

class HotkeyRegistrar : public QObject{
        Q_OBJECT
        public:
                explicit HotkeyRegistrar(QObject *parent = nullptr);
                ~HotkeyRegistrar();
                void registerHotkey();

        private slots:
                void onAboutToQuit();
                void onBindFinished(QDBusPendingCallWatcher *watcher);
                void onActivated(const QString &shortcut, const QVariantMap &timestamp);
                void onSessionCreated(QDBusPendingCallWatcher *watcher);

        private:
                void unbindShortcuts();
                void destroySession();
                bool m_registrar = false;
                QString m_sessionPath;
};
