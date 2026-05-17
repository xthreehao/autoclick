#include "head.h"
#include <QPalette>

MyCustomSlider::MyCustomSlider(Qt::Orientation orientation, QWidget *parent):QSlider(orientation, parent){
        m_displayLabel=new QLabel(this);
        m_displayLabel->setFixedSize(QSize(20,30));

        m_displayLabel->setAutoFillBackground(true);
        QPalette palette;
        palette.setColor(QPalette::Window, Qt::white);
        m_displayLabel->setPalette(palette);

        m_displayLabel->setAlignment(Qt::AlignCenter);

        m_displayLabel->setVisible(false);
        m_displayLabel->move(0,-m_displayLabel->height());
}

MyCustomSlider::~MyCustomSlider(){}

void MyCustomSlider::mousePressEvent(QMouseEvent *event){
        if(!m_displayLabel->isVisible()){
                m_displayLabel->setVisible(true);
                m_displayLabel->setText(QString::number(this->value()));
        }

        QSlider::mousePressEvent(event);
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event){
        if(m_displayLabel->isVisible()){
                m_displayLabel->setVisible(false);
        }
        QSlider::mouseReleaseEvent(event);
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event){
        m_displayLabel->setText(QString::number(this->value()));
        m_displayLabel->move((this->width()-m_displayLabel->width())*this->
                                value()/(this->maximum()-this->minimum()),-m_displayLabel->height());
        QSlider::mouseMoveEvent(event);
}