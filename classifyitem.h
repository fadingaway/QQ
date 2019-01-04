#ifndef CLASSIFYITEM_H
#define CLASSIFYITEM_H

#include <QWidget>
#include "QPushButton"
#include "QLabel"
#include "QListWidget"
#include "newfrienditem.h"
#include "QListWidgetItem"
#include "contactitem.h"
#include "listwidget.h"

class ContactItem;
class ListWidget;
struct listViewDetails
{
    QString userAcctId;
    QString userIconPath;
    QString userName;
    QString userSignature;
    int totalCommenFriends;
    int userStatus;
};

class classifyItem : public QWidget
{
    Q_OBJECT
public:
    explicit classifyItem(QWidget *parent,
                          uint itemId,
                          QString classifyName,
                          QListView::Flow layoutMode,
                          int width);
    void getActiveAndTotalCnt();
    void setLayoutMode(QListView::Flow layoutMode);
    void addListViewItem(QWidget *widget);
    void getDetailDataFromDatabase();
    void AddFriend(QString acctId);
    QString getClassifyName();
signals:
    void listViewShowed(QSize size);
    void listViewItemSelected(QListWidgetItem *item);
public slots:
    void setButtonArrowIcon();
    void showListViewDetail();
    void scrollToRight();
    void scrollToLeft();
    void onListViewItemClicked(QListWidgetItem *item);
    void deleteNewFrientItem(QString acctNo);
    void setItemShowStyle(QListWidgetItem *item);
    void resetItemToNormal();
private:
    QLabel *buttonArrow;
    QPushButton *headerButton;
    QLabel *name;
    QLabel *activeVsTotal;
    QPushButton *buttonMore;
    QPushButton *showMoreLeftButton;
    QPushButton *showMoreRightButton;
    QString classifyName;
    int activeCnt = 0;
    int totalCount= 0;
    ListWidget *listView;
    bool isButtonArrowPressed = false;
    QListView::Flow layoutMode;
    QList<listViewDetails> userDatas;
    int firstVisibleIndex = 0; //index 从0开始计数
    int contactitem_layoutMode = 1;
    uint itemId;
    QSize normalContactItemSizeHint;
};

#endif // CLASSIFYITEM_H
