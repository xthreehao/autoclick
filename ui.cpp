#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QSlider>

int ui()
{
    QApplication app();

    QWidget window;
    window.setWindowTitle("鼠标弘");
    window.resize(300,200);

    QSlider *slider = new QSlider(Qt::Horizontal);

    slider ->setRange(1 , 100);
    slider ->setValue(18);
    
}