#ifndef CONTACTTAB_H
#define CONTACTTAB_H

#include <QWidget>
#include "QPushButton"
#include "classifyitem.h"
#include "QToolBox"
#include "QScrollArea"
class classifyItem;
struct summaryInfo
{
    uint itemId;
    QString classifyItemName;
    QString totalItem;
    bool isSystemDefaul;
    QString activeItem;
};
class ContactTab : public QWidget
{
    Q_OBJECT
public:
    ContactTab(QWidget *parent, int width, int height);
    QList<summaryInfo> getFriendClassifyInfoFromDatabase();

signals:

public slots:
    void adjustComponentSizeHint(QSize size);
    void resetRestClassifyItem(QListWidgetItem *item);

private:
    QPushButton *friendButton;
    QPushButton *groupButton;
    QPushButton *addNewButton;
    QMenu *groupMenu;
    QMenu *rightClickMenu;
    QListWidget *content;
    QScrollArea *scroll;
};

#endif // CONTACTTAB_H
