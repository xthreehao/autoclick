#include "head.h"
#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDebug>
#include <QMessageBox>

HotkeyRegistrar::HotkeyRegistrar(QObject *parent) : QObject(parent){
        connect(qApp, &QApplication::aboutToQuit, this, &HotkeyRegistrar::onAboutToQuit);
}   

HotkeyRegistrar::~HotkeyRegistrar(){
        if(m_registrar){
                unbindShortcuts();
                destroySession();
        }
}

void HotkeyRegistrar::onAboutToQuit(){
        if(m_registrar){
                qDebug()<<"Pragram exiting, unregstering global hotkey...";
                unbindShortcuts();
                destroySession();
                m_registrar = false;
        }
}

void HotkeyRegistrar::registerHotkey(){
        QDBusInterface portal ("org.freedesktop.portal.Desktop",
                                "/org/freedesktop/portal/desktop",
                                "org.freedesktop.portal.GlobalShortcuts",
                                QDBusConnection::sessionBus());
        if(!portal.isValid()){
            QMessageBox::critical(nullptr , "错误" , "无法连接到xdg-desktop-portal,请确保已安装。");
            return;
        }
        
        QDBusPendingCall call = portal.asyncCall("CreateSession" , QVariantMap());
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call ,this);
        connect(watcher, &QDBusPendingCallWatcher::finished, this, 
        &HotkeyRegistrar::onSessionCreated);
}

void HotkeyRegistrar::onSessionCreated(QDBusPendingCallWatcher *watcher){
        QDBusPendingReply<QDBusObjectPath> reply = *watcher;
        if(reply.isError()){
                QMessageBox::critical(nullptr, "注册失败" , "创建会话失败"+reply.error().message()); 
                watcher->deleteLater();
                return;
        }
        qDebug() << "Session created:" <<m_sessionPath;

        QDBusInterface shortcutInterface("org.freedesktop.portal.Desktop",
                                        m_sessionPath,
                                        "org.freedesktop.portal.GlobalShortcuts",
                                        QDBusConnection::sessionBus());
        QVariantList shortcuts;
        QVariantMap shortcut;
        shortcut["key"] = "r";
        shortcut["description"] = "Toggle Auto Click";
        shortcuts << QVariant::fromValue(shortcut);
        QVariantMap options;
        QDBusPendingCall bindCall = shortcutInterface.asyncCall("BindShortcuts", shortcuts,options);
        QDBusPendingCallWatcher *bindWatcher = new QDBusPendingCallWatcher(bindCall,this);
        connect(bindWatcher , &QDBusPendingCallWatcher::finished, this, &HotkeyRegistrar::onBindFinished);
}

void HotkeyRegistrar::onBindFinished(QDBusPendingCallWatcher *watcher){
        QDBusPendingReply<> reply = *watcher;
        if(reply.isError()){
                QMessageBox::critical(nullptr , "注册失败" , "绑定快捷键失败"+reply.error().message());
        }
        else{
                m_registrar = true;
                QMessageBox::information(nullptr , "成功" , "全局快捷键R已注册\n现在按R键即可切换连点");
                QDBusConnection::sessionBus().connect(
                    "org.freedesktop.portal.Desktop",
                    "/org/freedesktop/portal/desktop",
                    "org.freedesktop.portal.GlobalShortcuts",
                    "Activated",
                    this,
                    SLOT(onActivated(QString,QVariantMap))
                );
                watcher->deleteLater();
        }
}

void HotkeyRegistrar::onActivated(const QString &shortcut, const QVariantMap &timestamp){
        Q_UNUSED(timestamp);
        if(shortcut == "r"){
                qDebug() << "R key pressed globally";
                globalRPressed = true;
        }
}

void HotkeyRegistrar::unbindShortcuts(){
        if(m_sessionPath.isEmpty()) return;
        QDBusInterface shotcutInterface("org.freedesktop.portal.Desktop",
                                        m_sessionPath,
                                        "org.freedesktop.portal.GlobalShortcuts",
                                        QDBusConnection::sessionBus());
        QVariantList shortcutsToUnbind;
        //TODO:将r设置特定的按键
        shortcutsToUnbind << QVariant::fromValue(QStringList{"r"});
        QDBusPendingCall call = shotcutInterface.asyncCall("UnbindShortcuts", shortcutsToUnbind);
}

void HotkeyRegistrar::destroySession(){
        if(m_sessionPath.isEmpty())return;
        QDBusInterface sessionInterface("org.freedesktop.portal.Desktop",
                                        m_sessionPath,
                                        "org.freedesktop.portal.Session",
                                        QDBusConnection::sessionBus());
        sessionInterface.asyncCall("close");
}