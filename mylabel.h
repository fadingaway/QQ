#ifndef MYLABEL_H
#define MYLABEL_H

#include "QLabel"
#include "QMouseEvent"
class MyLabel:public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent=Q_NULLPTR);
    ~MyLabel();
protected:
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusOutEvent(QFocusEvent *ev);
public:
    void setEnterPic(QPixmap &src);
    void setLeavePic(QPixmap &src);
signals:
    void clicked();
    void hover();
private:
    QPixmap hoverPic;
    QPixmap normalPic;
};


#endif // MYLABEL_H

