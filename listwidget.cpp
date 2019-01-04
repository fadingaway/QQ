#include "listwidget.h"
#include "QWheelEvent"

ListWidget::ListWidget(QWidget *parent): QListWidget(parent)
{

}
ListWidget::~ListWidget()
{

}

void ListWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
