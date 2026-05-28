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
#include <QString>
#include <QSlider>
#include <random>

float getRandom(int min, int max);

MainWindow::MainWindow(){
        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);

        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0,0,0,0);

        listWidget = new QListWidget;
        listWidget->addItem(tr("右键连点"));
        listWidget->addItem(tr("左键连点"));
        listWidget->addItem(tr("波动点击"));
        listWidget->setFixedWidth(100);

        stackedWidget = new QStackedWidget;

        createPages();

        connect(listWidget, &QListWidget::currentItemChanged,
            this, &MainWindow::changePage);

        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(stackedWidget ,1);
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

        //波动点击页
        QWidget *page3 = new QWidget;
        QVBoxLayout *layout3 = new QVBoxLayout(page3);
        upToDown = new RangeSlider(Qt::Horizontal, RangeSlider::DoubleHandles, page3);
        upToDown->SetRange(-500, 500);
        upToDown->SetLowerValue(-100);
        upToDown->SetUpperValue(100);

        number = new QLabel;
        number->setText(QString("波动范围:[%1, %2]")
            .arg(upToDown->GetLowerValue()/100.0, 0, 'f', 2)
            .arg(upToDown->GetUpperValue()/100.0, 0, 'f', 2));
        connect(upToDown, &RangeSlider::lowerValueChanged, this, &MainWindow::onWaveChanged);
        connect(upToDown, &RangeSlider::upperValueChanged, this, &MainWindow::onWaveChanged);

        layout3->addWidget(upToDown);
        layout3->addWidget(number);

        stackedWidget->addWidget(page1);
        stackedWidget->addWidget(page2);
        stackedWidget->addWidget(page3);
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
                        double cps = clickSlider->value() + getRandom(upToDown->GetLowerValue(), upToDown->GetUpperValue()) / 100.0;
                        if (cps < 1.0) cps = 1.0;
                        timer->start(static_cast<int>(1000.0 / cps));
                        clickButton->setText("右键连点中...");
                } else {
                        timer->stop();
                        clickButton->setText("按R开启右键连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [=](){
                DoOneClick(false);
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
                        double cps = slider2->value() + getRandom(upToDown->GetLowerValue(), upToDown->GetUpperValue()) / 100.0;
                        if (cps < 1.0) cps = 1.0;
                        timer->start(static_cast<int>(1000.0 / cps));
                        button2->setText("左键连点中...");
                } else {
                        timer->stop();
                        button2->setText("按Tab开启左键连点");
                }
        });

        QObject::connect(timer, &QTimer::timeout, [=](){
                DoOneClick(true);
        });

        QObject::connect(this, &QObject::destroyed, [=](){
                delete clicking;
        });
}

void MainWindow::onWaveChanged(){
        number->setText(QString("波动范围:[%1, %2]")
            .arg(upToDown->GetLowerValue()/100.0, 0, 'f', 2)
            .arg(upToDown->GetUpperValue()/100.0, 0, 'f', 2));
}

float getRandom(int min ,int max){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distrib(min, max);
        return distrib(gen);
}