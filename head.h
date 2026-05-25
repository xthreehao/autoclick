#pragma once
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>

class KeyboardMonitor;

void ui(KeyboardMonitor *rightMonitor, KeyboardMonitor *leftMonitor);

struct Log {
        void logERROR(const char* a);
        void logWARNING(const char* a);
        void logINFO(const char* a);
};

class MyCustomSlider : public QSlider{
        public:
                MyCustomSlider(Qt::Orientation orientation, QWidget *parent=0);
                ~MyCustomSlider();

        protected:
                virtual void mousePressEvent(QMouseEvent *event);
                virtual void mouseReleaseEvent(QMouseEvent *event);
                virtual void mouseMoveEvent(QMouseEvent *event);

        private:
                QLabel* m_displayLabel;
};

class MainWindow : public QMainWindow{
        Q_OBJECT

public :
        MainWindow();
        void setMonitorright(KeyboardMonitor *monitor);
        void setMonitorleft(KeyboardMonitor *monitor);
private:
        QListWidget *listWidget;
        QStackedWidget *stackedWidget;
        // 右键连点（page1）
        QPushButton *clickButton;
        MyCustomSlider *clickSlider;
        // 左键连点（page2）
        QPushButton *button2;
        MyCustomSlider *slider2;
private slots:
        void createPages();
        void changePage(QListWidgetItem *current,
        QListWidgetItem *previous);
};
void DoOneClick();
