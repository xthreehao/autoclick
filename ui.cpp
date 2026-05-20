#include "head.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QShortcut>
#include <QTimer>

int ui(int argc, char *argv[])
{
        QApplication app(argc , argv);

        QWidget window;
        window.setWindowTitle("鼠标宏");
        window.resize(300,200);
        
        MyCustomSlider *slider = new MyCustomSlider(Qt::Horizontal, &window);
        slider ->setRange(1 , 100);
        slider ->setValue(50);

        QPushButton *button = new QPushButton();
        button ->setText("按R开启连点");

        QVBoxLayout *layout = new QVBoxLayout;

        layout->addWidget(slider);
        layout->addWidget(button);

        window.setLayout(layout);

        QTimer *timer = new QTimer(&window);

        QShortcut *shortcut = new QShortcut(Qt::Key_R, &window);
        QObject::connect(shortcut, &QShortcut::activated, [&](){
                if(globalRPressed){
                        int cps = slider->value();
                        timer->start(1000 / cps);
                        button->setText("连点中,按R关闭");
                } else {
                        timer->stop();
                        button->setText("按R开启连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [&](){
                DoOneClick();
        });

        window.show();

        return app.exec();
}
