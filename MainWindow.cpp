#include "head.h"
#include "keyboard_monitor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QListWidget>
#include <QPushButton>
#include <QObject>
#include <QTimer>

MainWindow::MainWindow(){
        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);

        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0,0,0,0);

        listWidget = new QListWidget;
        listWidget->addItem(tr("右键连点"));
        listWidget->addItem(tr("左键连点"));
        listWidget->setFixedWidth(100);

        stackedWidget = new QStackedWidget;

        createPages();

        connect(listWidget, &QListWidget::currentItemChanged,
            this, &MainWindow::changePage);

        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(stackedWidget , 1);
}

void MainWindow::createPages(){
        // 右键连点页
        QWidget *page1 = new QWidget;
        QVBoxLayout *layout1 = new QVBoxLayout(page1);

        clickSlider = new MyCustomSlider(Qt::Horizontal, page1);
        clickSlider->setRange(1, 100);
        clickSlider->setValue(50);

        clickButton = new QPushButton("按R开启右键连点", page1);

        layout1->addWidget(clickSlider);
        layout1->addWidget(clickButton);

        // 左键连点页
        QWidget *page2 = new QWidget;
        QVBoxLayout *layout2 = new QVBoxLayout(page2);

        slider2 = new MyCustomSlider(Qt::Horizontal, page2);
        slider2->setRange(1, 100);
        slider2->setValue(50);

        button2 = new QPushButton("按Tab开启左键连点", page2);

        layout2->addWidget(slider2);
        layout2->addWidget(button2);

        stackedWidget->addWidget(page1);
        stackedWidget->addWidget(page2);
}

void MainWindow::changePage(QListWidgetItem *current, 
        QListWidgetItem *previous){
                if(!current)return;
                int index = listWidget->row(current);
                stackedWidget->setCurrentIndex(index);
}
void MainWindow::setMonitorright(KeyboardMonitor *monitor){
        QTimer *timer = new QTimer(this);
        bool *clicking = new bool(false);

        QObject::connect(monitor, &KeyboardMonitor::shortcutTriggered, this, [=](){
                *clicking = !*clicking;
                if (*clicking) {
                        timer->start(1000 / clickSlider->value());
                        clickButton->setText("连点中...");
                } else {
                        timer->stop();
                        clickButton->setText("按R开启右键连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [=](){
                DoOneClick();
        });

        QObject::connect(this, &QObject::destroyed, [=](){
                delete clicking;
        });
}

void MainWindow::setMonitorleft(KeyboardMonitor *monitor){
        QTimer *timer = new QTimer(this);
        bool *clicking = new bool(false);

        QObject::connect(monitor, &KeyboardMonitor::shortcutTriggered, this, [=](){
                *clicking = !*clicking;
                if (*clicking) {
                        timer->start(1000 / slider2->value());
                        button2->setText("连点中...");
                } else {
                        timer->stop();
                        button2->setText("按Tab开启左键连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [=](){
                DoOneClick();
        });

        QObject::connect(this, &QObject::destroyed, [=](){
                delete clicking;
        });
}