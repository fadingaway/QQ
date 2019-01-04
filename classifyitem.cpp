#include "classifyitem.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QDesktopServices"
#include "QUrl"
#include "newfrienditem.h"
#include "QListWidgetItem"
#include "QListWidget"
#include "contactitem.h"
#include "QDebug"
#include "QSizePolicy"
#include "QEvent"
#include "QWheelEvent"
#include "listwidget.h"

classifyItem::classifyItem(QWidget *parent,
                           uint itemId,
                           QString classifyName,
                           QListView::Flow layoutMode,
                           int width) : QWidget(parent)
{
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(width,400);
    this->classifyName = classifyName;
    this->layoutMode = layoutMode;
    this->itemId = itemId;
    this->setStyleSheet("font-family:微软雅黑;font-size:9pt;");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    headerButton = new QPushButton(this);
    QHBoxLayout *titleLayout = new QHBoxLayout;

    QHBoxLayout *headerLayout = new QHBoxLayout;
    buttonArrow  = new QLabel(this);
    buttonArrow->setFixedSize(25,25);
    buttonArrow->setAttribute(Qt::WA_TranslucentBackground);
    buttonArrow->setPixmap(QPixmap(":/Image/right.png"));


    name = new QLabel;
    name->setFixedHeight(25);
    name->setText(classifyName);
    name->setAttribute(Qt::WA_TranslucentBackground);
    name->setStyleSheet("font-family:微软雅黑;font-size:9pt;");

    activeVsTotal = new QLabel(this);
    getActiveAndTotalCnt();
    activeVsTotal->setText(QString("%1/%2").arg(activeCnt).arg(totalCount));
    activeVsTotal->setFixedHeight(25);
    activeVsTotal->setAttribute(Qt::WA_TranslucentBackground);

    buttonMore = new QPushButton(this);
    buttonMore->setText("更多");
    buttonMore->setFlat(true);
    buttonMore->setFixedSize(25,25);
    buttonMore->setCursor(Qt::PointingHandCursor);
    buttonMore->setAttribute(Qt::WA_TranslucentBackground);
    buttonMore->setStyleSheet("color:blue;border-style:none;");
    connect(buttonMore, &QPushButton::clicked, this, [=]{QDesktopServices::openUrl(QUrl("http://www.qq.com"));});

    headerLayout->addWidget(buttonArrow, Qt::AlignLeft);
    headerLayout->addWidget(name);
    headerLayout->addWidget(activeVsTotal);
    headerLayout->addSpacing(250);
    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->setSpacing(3);
    headerButton->setLayout(headerLayout);
    headerButton->setFixedHeight(25);
    headerButton->setFlat(true);
    headerButton->setStyleSheet("QPushButton{border-style:none;}"
                                "QPushButton::hover{background-color:lightgray;}");
    connect(headerButton, &QPushButton::clicked, this, &classifyItem::setButtonArrowIcon);
    connect(headerButton, &QPushButton::clicked, this, &classifyItem::showListViewDetail);

    titleLayout->addWidget(headerButton, Qt::AlignLeft);
    titleLayout->addWidget(buttonMore, Qt::AlignRight);
    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->setSpacing(3);

    QHBoxLayout *contentLayout = new QHBoxLayout;

    listView = new ListWidget(this);
    listView->setFocusPolicy(Qt::NoFocus);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setResizeMode(QListView::Adjust);
    listView->setAutoScroll(false);
    listView->setContentsMargins(0,0,0,0);
    if(layoutMode == QListView::TopToBottom)
    {
        listView->setSpacing(0);
        //listView->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    else
    {
        listView->setSpacing(2);
        listView->setSelectionMode(QAbstractItemView::NoSelection);

    }
    listView->hide();
    listView->setMouseTracking(true);
    listView->setStyleSheet("QListWidget{border-style:none;background:white;}"
                            "QListWidget::item{background:white;}"
                            "QListWidget::item:selected{background:lightgray;}");
    connect(listView, &QListWidget::itemClicked, this, &classifyItem::listViewItemSelected);

    showMoreLeftButton = new QPushButton(this);
    showMoreLeftButton->setFlat(true);
    showMoreLeftButton->setStyleSheet("QPushButton{border-style:none;background-image:url(:/Image/left.png);}"
                                      "QPushButton::hover{background-image:url(:/Image/left_hover.png);}");
    connect(showMoreLeftButton, &QPushButton::clicked, this, &classifyItem::scrollToLeft);
    showMoreLeftButton->hide();
    QSizePolicy policy;
    policy.setRetainSizeWhenHidden(true);
    showMoreLeftButton->setSizePolicy(policy);

    showMoreRightButton = new QPushButton(this);
    showMoreRightButton->setFlat(true);
    showMoreRightButton->setStyleSheet("QPushButton{border-style:none;background-image:url(:/Image/right.png);}"
                                      "QPushButton::hover{background-image:url(:/Image/right_hover.png);}");
    connect(showMoreRightButton, &QPushButton::clicked, this, &classifyItem::scrollToRight);
    showMoreRightButton->hide();
    showMoreRightButton->setSizePolicy(policy);

    contentLayout->addWidget(showMoreLeftButton);
    contentLayout->addWidget(listView);
    contentLayout->addWidget(showMoreRightButton);
    contentLayout->setContentsMargins(0,5,0,0);
    contentLayout->setSpacing(0);

    setLayoutMode(layoutMode);
    getDetailDataFromDatabase();

    mainLayout->addLayout(titleLayout, Qt::AlignTop);
    mainLayout->addLayout(contentLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    this->setLayout(mainLayout);
    this->setFixedHeight(30);
    this->setMouseTracking(true);
}

void classifyItem::setLayoutMode(QListView::Flow layoutMode)
{
    if(layoutMode == QListView::LeftToRight)
    {
        listView->setFlow(QListView::LeftToRight);
        showMoreLeftButton->setFixedSize(20,20);
        showMoreRightButton->setFixedSize(20,20);
        activeVsTotal->hide();
        buttonMore->show();
    }
    else
    {
        listView->setFlow(QListView::TopToBottom);
        activeVsTotal->show();
        buttonMore->hide();
    }
}

void classifyItem::setButtonArrowIcon()
{
    if(isButtonArrowPressed)
    {
        isButtonArrowPressed = false;
    }
    else
    {
        isButtonArrowPressed = true;
    }

    if(isButtonArrowPressed)
    {
        buttonArrow->setPixmap(QPixmap(":/Image/down.png"));
    }
    else
    {
        buttonArrow->setPixmap(QPixmap(":/Image/right.png"));
    }
}

void classifyItem::getActiveAndTotalCnt()
{
    activeCnt = 1;
    totalCount = 5;
}

void classifyItem::addListViewItem(QWidget *widget)
{
    QListWidgetItem *listItem = new QListWidgetItem(listView);
    listItem->setSizeHint(widget->size());
    listView->setItemWidget(listItem, widget);
    listView->installEventFilter(this);
}

void classifyItem::showListViewDetail()
{
    if(isButtonArrowPressed)
    {
        listView->show();
        firstVisibleIndex = 0;
        QSizePolicy policy;
        if(layoutMode == QListView::TopToBottom)
        {
            policy.setRetainSizeWhenHidden(false);
            showMoreRightButton->setSizePolicy(policy);
            showMoreLeftButton->setSizePolicy(policy);
            showMoreLeftButton->hide();
            showMoreRightButton->hide();
        }
        else if(userDatas.size()<=4)
        {
            policy.setRetainSizeWhenHidden(true);
            showMoreRightButton->setSizePolicy(policy);
            showMoreLeftButton->setSizePolicy(policy);
            showMoreLeftButton->hide();
            showMoreRightButton->hide();
        }
        else
        {
            policy.setRetainSizeWhenHidden(true);
            showMoreRightButton->setSizePolicy(policy);
            showMoreLeftButton->setSizePolicy(policy);
            showMoreLeftButton->hide();
            showMoreRightButton->show();
        }
        int itemCount = listView->count();
        int height = 0;
        if(layoutMode == QListView::LeftToRight)
        {
            height = 105 + 40;
        }
        else
        {
            if(contactitem_layoutMode == 1)
            {
                height = itemCount * 45  //contactitem height
                        + 10        //classifyItem contentmargin
                        + 30;        //header height
            }
            else if(contactitem_layoutMode == 2)
            {
                height = itemCount * 55     //contactItem height
                        + 10               //classifyItem contentmargin
                        + 30;               //header height
            }
        }
        this->setFixedHeight(height);
    }
    else
    {
        listView->clearSelection();
        listView->hide();
        this->setFixedHeight(30);
        if(layoutMode == QListView::LeftToRight)
        {
            QSizePolicy policy;
            policy.setRetainSizeWhenHidden(true);
            showMoreRightButton->setSizePolicy(policy);
            showMoreLeftButton->setSizePolicy(policy);
            showMoreLeftButton->hide();
            showMoreRightButton->hide();
        }
        else
        {
            QSizePolicy policy;
            policy.setRetainSizeWhenHidden(false);
            showMoreRightButton->setSizePolicy(policy);
            showMoreLeftButton->setSizePolicy(policy);
            showMoreLeftButton->hide();
            showMoreRightButton->hide();
        }
    }
    //清除选中状态
    if(!normalContactItemSizeHint.isEmpty())
    {
        for(int i = 0; i<listView->count(); i++)
        {
            ContactItem *l_contactitem = qobject_cast<ContactItem *> (listView->itemWidget(listView->item(i)));
            l_contactitem->setLayoutMode(1);
            listView->item(i)->setSizeHint(normalContactItemSizeHint);
        }
    }
    emit listViewShowed(this->size());
}

void classifyItem::getDetailDataFromDatabase()
{
    userDatas.clear();
    listViewDetails listViewDetailItem;
    listViewDetailItem.userAcctId = QString("100001");
    listViewDetailItem.userIconPath = QString(":/Image/head_1.jpg");
    listViewDetailItem.userName = QString("一切随风");
    listViewDetailItem.userStatus = 1;
    listViewDetailItem.userSignature = QString("[离线请留言] You can be as mad as a mad dog as the thing it went, you can swear and curse the fate, but when it comes to an end, you have to let it go.");
    listViewDetailItem.totalCommenFriends = 4;
    userDatas.append(listViewDetailItem);

    listViewDetailItem.userAcctId = QString("100002");
    listViewDetailItem.userIconPath = QString(":/Image/head_2.jpg");
    listViewDetailItem.userName = QString("2");
    listViewDetailItem.userStatus = 2;
    listViewDetailItem.userSignature = QString("you can be as mad as a mad god as the things went, you can swear or curse the fate, but when "
                                               "it comes to an end, you have to let it go.");
    listViewDetailItem.totalCommenFriends = 5;
    userDatas.append(listViewDetailItem);

    listViewDetailItem.userAcctId = QString("100003");
    listViewDetailItem.userIconPath = QString(":/Image/head_3.jpg");
    listViewDetailItem.userName = QString("3");
    listViewDetailItem.userStatus = 1;
    listViewDetailItem.userSignature = QString("I wish you can experience things I never do.");
    listViewDetailItem.totalCommenFriends = 6;
    userDatas.append(listViewDetailItem);

    listViewDetailItem.userAcctId = QString("100004");
    listViewDetailItem.userIconPath = QString(":/Image/head_4.jpg");
    listViewDetailItem.userName = QString("4");
    listViewDetailItem.userSignature = QString("Benjamin Button");
    listViewDetailItem.userStatus = 2;
    listViewDetailItem.totalCommenFriends = 7;
    userDatas.append(listViewDetailItem);

    listViewDetailItem.userAcctId = QString("100005");
    listViewDetailItem.userIconPath = QString(":/Image/head_5.jpg");
    listViewDetailItem.userName = QString("5");
    listViewDetailItem.userStatus = 1;
    listViewDetailItem.userSignature = QString("Justis will never be late.");
    listViewDetailItem.totalCommenFriends = 5;
    userDatas.append(listViewDetailItem);

    if(layoutMode == QListView::LeftToRight)
    {
        listView->setFixedSize(4*78,115);
        listView->setContentsMargins(5,5,5,5);
        listView->setStyleSheet("border-style:none;font-size:8pt;");
        for(int i = 0; i< userDatas.size()&& i < 4; i++)
        {
            newFriendItem *friendItem = new newFriendItem(listView,
                                                          userDatas.at(i).userAcctId,
                                                          userDatas.at(i).userIconPath,
                                                          userDatas.at(i).userName,
                                                          userDatas.at(i).totalCommenFriends);
            addListViewItem(friendItem);
            connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
            connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
        }
    }
    else
    {
        for(int i = 0; i < userDatas.size(); i++)
        {
            contactitem_layoutMode = 1;
            ContactItem *contactItem = new ContactItem(listView,
                                                       this->width(),
                                                       userDatas.at(i).userIconPath,
                                                       userDatas.at(i).userName,
                                                       userDatas.at(i).userStatus,
                                                       userDatas.at(i).userSignature,
                                                       contactitem_layoutMode);
            addListViewItem(contactItem);
            connect(listView, &QListWidget::itemClicked, this, setItemShowStyle);
        }
    }
}

void classifyItem::AddFriend(QString acctId)
{
    listView->clear();
    for(int i = 0; i<userDatas.size(); i++)
    {
        if(acctId == userDatas[i].userAcctId)
        {
            userDatas.removeAt(i);
            break;
        }
    }
    for(int i = firstVisibleIndex; i< userDatas.size() && i < firstVisibleIndex+4; i++)
    {
        newFriendItem *friendItem = new newFriendItem(listView,
                                                      userDatas.at(i).userAcctId,
                                                      userDatas.at(i).userIconPath,
                                                      userDatas.at(i).userName,
                                                      userDatas.at(i).totalCommenFriends);
        addListViewItem(friendItem);
        connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
        connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
    }
    if(firstVisibleIndex + 3 >= userDatas.size()-1)
    {
        showMoreRightButton->hide();
    }
}

void classifyItem::scrollToRight()
{
    if(firstVisibleIndex ==0)
    {
        if(firstVisibleIndex + 3 >= userDatas.size() - 1)
        {
            showMoreRightButton->hide();
        }
        if(userDatas.size() - 4 >= 4)
        {
            listView->clear();
            for(int i = 4; i< userDatas.size() && i < 8; i++)
            {
                newFriendItem *friendItem = new newFriendItem(listView,
                                                              userDatas.at(i).userAcctId,
                                                              userDatas.at(i).userIconPath,
                                                              userDatas.at(i).userName,
                                                              userDatas.at(i).totalCommenFriends);
                addListViewItem(friendItem);
                connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
                connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
            }
            firstVisibleIndex = 4;
            showMoreRightButton->show();
            showMoreLeftButton->show();
        }
        else if(userDatas.size() - 4 > 0)
        {
            listView->clear();
            for(int i = userDatas.size() - 4; i< userDatas.size(); i++)
            {
                newFriendItem *friendItem = new newFriendItem(listView,
                                                              userDatas.at(i).userAcctId,
                                                              userDatas.at(i).userIconPath,
                                                              userDatas.at(i).userName,
                                                              userDatas.at(i).totalCommenFriends);
                addListViewItem(friendItem);
                connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
                connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
            }
            firstVisibleIndex = userDatas.size() - 4;
            showMoreRightButton->hide();
            showMoreLeftButton->show();
        }
    }
    else
    {
        if(userDatas.size()-1 - firstVisibleIndex >= 4)
        {
            listView->clear();
            for(int i = firstVisibleIndex+4;
                i< userDatas.size() && i <firstVisibleIndex+8;
                i++)
            {
                newFriendItem *friendItem = new newFriendItem(listView,
                                                              userDatas.at(i).userAcctId,
                                                              userDatas.at(i).userIconPath,
                                                              userDatas.at(i).userName,
                                                              userDatas.at(i).totalCommenFriends);
                addListViewItem(friendItem);
                connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
                connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
            }
            firstVisibleIndex = firstVisibleIndex+4;
            if(userDatas.size()<=(firstVisibleIndex+4+1))
            {
                showMoreRightButton->hide();
            }
            showMoreLeftButton->show();
        }
        else if(userDatas.size()-1 - firstVisibleIndex > 0)
        {
            listView->clear();
            for(int i = userDatas.size()-4-1; i< userDatas.size(); i++)
            {
                newFriendItem *friendItem = new newFriendItem(listView,
                                                              userDatas.at(i).userAcctId,
                                                              userDatas.at(i).userIconPath,
                                                              userDatas.at(i).userName,
                                                              userDatas.at(i).totalCommenFriends);
                addListViewItem(friendItem);
                connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
                connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
            }
            firstVisibleIndex = userDatas.size()-4-1;
            showMoreRightButton->hide();
            showMoreLeftButton->show();
        }
    }
}

void classifyItem::scrollToLeft()
{
    if(firstVisibleIndex - 4 >= 0)
    {
        listView->clear();
        for(int i = firstVisibleIndex - 4; i < firstVisibleIndex; i++)
        {
            newFriendItem *friendItem = new newFriendItem(listView,
                                                          userDatas.at(i).userAcctId,
                                                          userDatas.at(i).userIconPath,
                                                          userDatas.at(i).userName,
                                                          userDatas.at(i).totalCommenFriends);
            addListViewItem(friendItem);
            connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
            connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
        }
        firstVisibleIndex = firstVisibleIndex - 4;
        if(firstVisibleIndex == 0)
        {
            showMoreLeftButton->hide();
        }
        showMoreRightButton->show();
    }
    else if(firstVisibleIndex < 4)
    {
        listView->clear();
        for(int i = 0; i < 4; i++)
        {
            newFriendItem *friendItem = new newFriendItem(listView,
                                                          userDatas.at(i).userAcctId,
                                                          userDatas.at(i).userIconPath,
                                                          userDatas.at(i).userName,
                                                          userDatas.at(i).totalCommenFriends);
            addListViewItem(friendItem);
            connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
            connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
        }
        firstVisibleIndex = 0;
        showMoreLeftButton->hide();
        showMoreRightButton->show();
    }
}

void classifyItem::onListViewItemClicked(QListWidgetItem *item)
{
    emit listViewItemSelected(item);
}

void classifyItem::deleteNewFrientItem(QString acctNo)
{
    listView->clear();
    for(int i = 0; i<userDatas.size(); i++)
    {
        if(acctNo == userDatas[i].userAcctId)
        {
            userDatas.removeAt(i);
            break;
        }
    }
    for(int i = firstVisibleIndex; i< userDatas.size() && i < firstVisibleIndex+4; i++)
    {
        newFriendItem *friendItem = new newFriendItem(listView,
                                                      userDatas.at(i).userAcctId,
                                                      userDatas.at(i).userIconPath,
                                                      userDatas.at(i).userName,
                                                      userDatas.at(i).totalCommenFriends);
        addListViewItem(friendItem);
        connect(friendItem, &newFriendItem::requestToAddFriend, this, &AddFriend);
        connect(friendItem, &newFriendItem::requestToDeleteItem, this, &classifyItem::deleteNewFrientItem);
    }
    if(firstVisibleIndex + 3 >= userDatas.size()-1)
    {
        showMoreRightButton->hide();
    }
}

void classifyItem::setItemShowStyle(QListWidgetItem *item)
{
    ContactItem *contactitem = qobject_cast<ContactItem *> (listView->itemWidget(item));

    if(normalContactItemSizeHint.isEmpty())
    {
        normalContactItemSizeHint = contactitem->size();
    }
    if(contactitem && contactitem->getLayoutMode() == 1)
    {
        contactitem->setLayoutMode(2);
    }
    for(int i = 0; i<listView->count(); i++)
    {
        QListWidgetItem *local_item = listView->item(i);
        ContactItem *l_contactitem = qobject_cast<ContactItem *> (listView->itemWidget(local_item));
        if(l_contactitem->getUserName() == contactitem->getUserName())
        {
            local_item->setSizeHint(contactitem->size());
        }
        else
        {
            l_contactitem->setLayoutMode(1);
            local_item->setSizeHint(normalContactItemSizeHint);
        }
    }
}

QString classifyItem::getClassifyName()
{
    return classifyName;
}

void classifyItem::resetItemToNormal()
{
    listView->clearSelection();
    for(int i = 0; i<listView->count(); i++)
    {
        QListWidgetItem *local_item = listView->item(i);
        ContactItem *l_contactitem = qobject_cast<ContactItem *> (listView->itemWidget(local_item));
        if(l_contactitem)
        {
            if(l_contactitem->getLayoutMode() == 2)
            {
                l_contactitem->setLayoutMode(1);
                local_item->setSizeHint(normalContactItemSizeHint);
            }
        }
    }
}



