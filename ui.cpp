#include "head.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

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
        button ->setText("a");

        QVBoxLayout *layout = new QVBoxLayout;

        layout->addWidget(slider);
        layout->addWidget(button);

        window.setLayout(layout);

        window.show();

        return app.exec();
}