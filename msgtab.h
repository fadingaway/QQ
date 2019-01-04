#ifndef MSGTAB_H
#define MSGTAB_H

#include <QWidget>
#include "incomingmsgitem.h"
#include "QListWidget"
struct msgItem
{
    bool isGroupInd;
    QString nickNameString;
    QString lastMsgDateTime;
    QString lastMsgFromUserName;
    QString lastMsgDetail;
    QString iconPath;
    int unreadMsgCnt;
    QString miniIconPath;
};
class MsgTab : public QWidget
{
    Q_OBJECT
public:
    explicit MsgTab(QWidget *parent = 0, int width = 0, int height = 0);
    void getInfoFromDatabase();
signals:
public slots:
    void onItemDoubleClicked();
    void onItemRightClicked();

private:
    QListWidget *listView;
    QList<msgItem> msgItems;
};

#endif // MSGTAB_H
