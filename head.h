#pragma once
#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
class KeyboardMonitor;

void ui(KeyboardMonitor *monitor);

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

void DoOneClick();
