#include "contacttab.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QScrollArea"
#include "QDebug"
#include "QScrollBar"

const int NEW_FRIEND_ITEMID = 100000001;
const int MY_DEVICE_ITEMID = 100000002;
const int PUBLIC_NUMBER_ITEM = 100000003;
const int STRANGER_ITEMID = 100000004;
const int BLACKLIST_ITEMID = 10000005;

ContactTab::ContactTab(QWidget *parent, int width, int height) : QWidget(parent)
{
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(width, height);
    this->setStyleSheet("background-color:white;");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *headerLayout = new QHBoxLayout;
    friendButton  = new QPushButton(this);
    friendButton->setText("好友");
    friendButton->setFlat(true);
    friendButton->setFixedSize(35,25);
    friendButton->setStyleSheet("font-family:微软雅黑;font-size:9pt;color:darkgray;");


    groupButton = new QPushButton(this);
    groupButton->setText("群聊");
    groupButton->setFlat(true);
    groupButton->setFixedSize(35,25);
    groupButton->setStyleSheet("font-family:微软雅黑;font-size:9pt;color:darkgray;");

    addNewButton = new QPushButton(this);
    addNewButton->setFlat(true);
    addNewButton->setFixedSize(25,25);
    addNewButton->setStyleSheet("background-image:url(:/Image/focus.png);");

    headerLayout->addWidget(friendButton, Qt::AlignLeft);
    headerLayout->addWidget(groupButton, Qt::AlignLeft);
    headerLayout->addSpacing(200);
    headerLayout->addWidget(addNewButton, Qt::AlignRight);
    headerLayout->setSpacing(3);
    headerLayout->setContentsMargins(0,5,5,5);

    content = new QListWidget(this);
    content->setSpacing(0);
    content->setFocusPolicy(Qt::NoFocus);
    content->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    content->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    content->setSelectionMode(QAbstractItemView::NoSelection);
    content->setMinimumHeight(this->height()-35);
    content->setFixedWidth(370);

    classifyItem *newFriendItem = new classifyItem(this,NEW_FRIEND_ITEMID, "新朋友", QListView::LeftToRight,this->width()-10);
    QListWidgetItem *item_1 = new QListWidgetItem(content);
    item_1->setSizeHint(newFriendItem->size());
    content->setItemWidget(item_1, newFriendItem);
    connect(newFriendItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
    connect(newFriendItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);

    classifyItem *deviceItem = new classifyItem(this, MY_DEVICE_ITEMID, "我的设备", QListView::TopToBottom, this->width()-10);
    QListWidgetItem *item_2 = new QListWidgetItem(content);
    item_2->setSizeHint(deviceItem->size());
    content->setItemWidget(item_2, deviceItem);
    connect(deviceItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
    connect(deviceItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);

    QList<summaryInfo> friendClassifyInfo = getFriendClassifyInfoFromDatabase();

    for(int i = 0; i< friendClassifyInfo.size(); i++)
    {
        QListWidgetItem *item_3 = new QListWidgetItem(content);
        classifyItem *friendItem = new classifyItem(this,
                                                    friendClassifyInfo[i].itemId,
                                                    friendClassifyInfo[i].classifyItemName,
                                                    QListView::TopToBottom,
                                                    this->width()-10);
        item_3->setSizeHint(friendItem->size());
        content->setItemWidget(item_3, friendItem);
        connect(friendItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
        connect(friendItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);
    }

    classifyItem *publicItem = new classifyItem(this, PUBLIC_NUMBER_ITEM, "公众号", QListView::TopToBottom, this->width()-10);
    QListWidgetItem *item_4 = new QListWidgetItem(content);
    item_4->setSizeHint(publicItem->size());
    content->setItemWidget(item_4, publicItem);
    connect(publicItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
    connect(publicItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);

    classifyItem *strangerItem = new classifyItem(this, STRANGER_ITEMID, "陌生人", QListView::TopToBottom, this->width()-10);
    QListWidgetItem *item_5 = new QListWidgetItem(content);
    item_5->setSizeHint(strangerItem->size());
    content->setItemWidget(item_5, strangerItem);
    connect(strangerItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
    connect(strangerItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);

    classifyItem *blacklistItem = new classifyItem(this, BLACKLIST_ITEMID, "黑名单", QListView::TopToBottom, this->width()-10);
    QListWidgetItem *item_6 = new QListWidgetItem(content);
    item_6->setSizeHint(blacklistItem->size());
    content->setItemWidget(item_6, blacklistItem);
    connect(blacklistItem, &classifyItem::listViewShowed, this, &ContactTab::adjustComponentSizeHint);
    connect(blacklistItem, &classifyItem::listViewItemSelected, this, &ContactTab::resetRestClassifyItem);

    content->setContentsMargins(0,0,0,0);
    scroll = new QScrollArea(this);
    scroll->setWidget(content);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setFixedWidth(370);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidgetResizable(true);
    scroll->verticalScrollBar()->setStyleSheet("QScrollBar::handle:vertical {\
                             background: rgb(215,215,215)\
                             border-radius:4px;\
                             border:2px solid dark;\
                             min-height: 20px;\
                             width:5px;}");
    QVBoxLayout *scrollLayout = new QVBoxLayout;
    scrollLayout->addWidget(scroll);
    scrollLayout->setSpacing(0);
    scrollLayout->setMargin(0);

    mainLayout->addLayout(headerLayout, Qt::AlignTop|Qt::AlignLeft);
    mainLayout->addLayout(scrollLayout);
    //mainLayout->addWidget(content);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    this->setLayout(mainLayout);
}

QList<summaryInfo> ContactTab::getFriendClassifyInfoFromDatabase()
{
    QList<summaryInfo> list;
    summaryInfo info;
    info.classifyItemName = QString("我的好友");
    info.itemId = 100000006;
    info.totalItem = 20;
    info.isSystemDefaul = false;
    info.activeItem = 10;
    list.append(info);
    return list;
}

void ContactTab::adjustComponentSizeHint(QSize size)
{
    qDebug()<<"adjustComponentSizeHint";
    classifyItem *item = qobject_cast<classifyItem *> (sender());
    for(int i = 0; i<content->count(); i++)
    {
        if(content->itemWidget(content->item(i)) == item)
        {
            content->item(i)->setSizeHint(size);
            break;
        }
    }
    scroll->setMinimumHeight(content->height());
}

void ContactTab::resetRestClassifyItem(QListWidgetItem *item)
{
    classifyItem *classifyitem = qobject_cast<classifyItem *> (sender());
    for(int i = 0; i<content->count(); i++)
    {
        classifyItem *tempClassifyItem = qobject_cast<classifyItem *>(content->itemWidget(content->item(i)));
        if(tempClassifyItem->getClassifyName() != classifyitem->getClassifyName())
        {
           tempClassifyItem->resetItemToNormal();
        }
    }
}
