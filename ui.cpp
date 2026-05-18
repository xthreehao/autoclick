#include "head.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QShortcut>

int ui(int argc, char *argv[])
{
        QApplication app(argc , argv);

        QWidget window;
        window.setWindowTitle("鼠标宏");
        window.resize(300,200);

        MyCustomSlider *slider = new MyCustomSlider(Qt::Horizontal, &window);
        slider ->setRange(1 , 100);
        slider ->setValue(18);

        QPushButton *button = new QPushButton();
        button ->setText("按R开启连点");

        QVBoxLayout *layout = new QVBoxLayout;

        layout->addWidget(slider);
        layout->addWidget(button);

        window.setLayout(layout);

        QShortcut *shortcut = new QShortcut(Qt::Key_R, &window);
        QObject::connect(shortcut, &QShortcut::activated, [&](){
                AutoClickRunning = !AutoClickRunning;
                button->setText(AutoClickRunning ? "连点中，按R关闭" : "按R开ue启连点");
                Autoclick(slider->value());
        });

        window.show();

        return app.exec();
}
