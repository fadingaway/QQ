#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "QListWidget"

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR);
    ~ListWidget();
    void wheelEvent(QWheelEvent *event);
};

#endif // LISTWIDGET_H
