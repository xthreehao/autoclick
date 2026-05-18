#include "head.h"
#include <QPalette>

MyCustomSlider::MyCustomSlider(Qt::Orientation orientation, QWidget *parent):QSlider(orientation, parent){
        m_displayLabel=new QLabel(this->window());
        m_displayLabel->setFixedSize(QSize(20,30));

        m_displayLabel->setAutoFillBackground(true);
        QPalette palette;
        palette.setColor(QPalette::Window, Qt::white);
        m_displayLabel->setPalette(palette);

        m_displayLabel->setAlignment(Qt::AlignCenter);

        m_displayLabel->setVisible(false);
}

MyCustomSlider::~MyCustomSlider(){}

void MyCustomSlider::mousePressEvent(QMouseEvent *event)
{
        QSlider::mousePressEvent(event);
        if (!m_displayLabel->isVisible()) {
                m_displayLabel->setVisible(true);
                m_displayLabel->setText(QString::number(this->value()));
                
                // 计算局部坐标
                int x = (width() - m_displayLabel->width()) * value() / (maximum() - minimum());
                int y = -m_displayLabel->height() - 2;   // 放在滑块上方，留2像素间距
                
                // 转换为窗口坐标
                QPoint windowPos = mapTo(this->window(), QPoint(x, y));
                m_displayLabel->move(windowPos);
        }
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event)
{
        QSlider::mouseMoveEvent(event);
        m_displayLabel->setText(QString::number(this->value()));
        
        int x = (width() - m_displayLabel->width()) * value() / (maximum() - minimum());
        int y = -m_displayLabel->height() - 2;
        QPoint windowPos = mapTo(this->window(), QPoint(x, y));
        m_displayLabel->move(windowPos);
    
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event){
        QSlider::mouseReleaseEvent(event);
        if(m_displayLabel->isVisible()){
                m_displayLabel->setVisible(false);
        }
}