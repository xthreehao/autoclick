#include <QApplication>
#include <QDebug>
#include <QThread>
#include <dbus/dbus.h>
#include <QTimer>

// 全局变量保存快捷键是否被按下
volatile bool globalRPressed = false;
DBusConnection *global_dbus_conn = nullptr;

// DBus 消息处理函数
static DBusHandlerResult filter_function(DBusConnection *connection, DBusMessage *message, void *user_data) {
    if (dbus_message_is_signal(message, "org.freedesktop.portal.GlobalShortcuts", "Activated")) {
        DBusError error;
        dbus_error_init(&error);

        const char *shortcut = nullptr;
        dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &shortcut, DBUS_TYPE_INVALID);

        if (shortcut && strcmp(shortcut, "r") == 0) {
            qDebug() << "R key pressed globally!";
            globalRPressed = true;
        }
    }
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int ChangeGlobalRPressed(QApplication &app) {
    // 初始化 DBus
    DBusError err;
    dbus_error_init(&err);
    global_dbus_conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        qCritical() << "DBus connection error:" << err.message;
        return 1;
    }

    // 添加消息过滤器
    dbus_connection_add_filter(global_dbus_conn, filter_function, nullptr, nullptr);
    dbus_connection_set_exit_on_disconnect(global_dbus_conn, false);

    qDebug() << "请确保已运行: gdbus call --session --dest org.freedesktop.portal.Desktop --object-path /org/freedesktop/portal/desktop --method org.freedesktop.portal.GlobalShortcuts.CreateSession '' {}";
    qDebug() << "然后使用返回的 session_handle 绑定快捷键 'r'";

    return 0;
}
