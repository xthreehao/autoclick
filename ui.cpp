#include "head.h"
#include "keyboard_monitor.h"
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QTimer>

void ui(KeyboardMonitor *monitor)
{
        QWidget *window = new QWidget();
        window->setWindowTitle("鼠标宏");
        window->resize(300, 200);
        window->setAttribute(Qt::WA_DeleteOnClose);

        MyCustomSlider *slider = new MyCustomSlider(Qt::Horizontal, window);
        slider->setRange(1, 100);
        slider->setValue(50);

        QPushButton *button = new QPushButton("按R开启连点");

        QVBoxLayout *layout = new QVBoxLayout(window);
        layout->addWidget(slider);
        layout->addWidget(button);

        QTimer *timer = new QTimer(window);
        bool *clicking = new bool(false);

        QObject::connect(monitor, &KeyboardMonitor::shortcutTriggered, window, [=](){
                *clicking = !*clicking;
                if (*clicking) {
                        timer->start(1000 / slider->value());
                        button->setText("连点中,按R关闭");
                } else {
                        timer->stop();
                        button->setText("按R开启连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [=](){
                DoOneClick();
        });

        QObject::connect(window, &QObject::destroyed, [=](){
                delete clicking;
        });

        window->show();
}
