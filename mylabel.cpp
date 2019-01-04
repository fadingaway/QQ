#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{
}

MyLabel::~MyLabel()
{

}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    event->accept();
}

void MyLabel::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    if(!hoverPic.isNull())
      this->setPixmap(hoverPic);
}

void MyLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    if(!normalPic.isNull())
        this->setPixmap(normalPic);
}

void MyLabel::setEnterPic(QPixmap &src)
{
    hoverPic = src;
}

void MyLabel::setLeavePic(QPixmap &src)
{
    normalPic = src;
}

void MyLabel::focusOutEvent(QFocusEvent *ev)
{
    this->hide();
}
