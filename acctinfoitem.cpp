#include "acctinfoitem.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QPixmap"
#include "QPushButton"
#include "QPainter"
#include "QListWidgetItem"

AcctInfoItem::AcctInfoItem(QWidget *parent) :QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    icon = new QLabel();
    icon->setFixedSize(35,35);

    nickName = new QLabel;
    nickName->setFont(QFont("微软雅黑",10));
    nickName->setFixedWidth(100);

    acctNo = new QLabel;
    acctNo->setFont(QFont("微软雅黑",10));
    acctNo->setFixedWidth(100);
    acctNo->setStyleSheet("color:Gray;");

    del = new QPushButton;
    del->setFlat(true);
    del->setFixedSize(32,32);
    del->setFocusPolicy(Qt::NoFocus);
    del->setCursor(Qt::PointingHandCursor);
    del->setStyleSheet("background-image:url(:/Image/delete.png);"
                       "border-style:none;"
                       "QPushButton::hover{background-color:red}");
    connect(del, &QPushButton::clicked, this, &AcctInfoItem::onDelButtonClicked);

    hLayout->addWidget(icon, Qt::AlignCenter|Qt::AlignLeading);
    QVBoxLayout *acctinfoLayout = new QVBoxLayout;
    acctinfoLayout->addWidget(nickName, Qt::AlignLeft);
    acctinfoLayout->addWidget(acctNo, Qt::AlignLeft);
    acctinfoLayout->setSpacing(0);
    acctinfoLayout->setContentsMargins(0,0,0,5);
    hLayout->addLayout(acctinfoLayout);
    hLayout->addWidget(del, Qt::AlignRight|Qt::AlignCenter);
    hLayout->setSpacing(0);
    hLayout->setStretch(1,0);
    hLayout->setContentsMargins(5,5,0,10);
    this->setLayout(hLayout);
    this->setFixedHeight(50);
}

AcctInfoItem::~AcctInfoItem()
{

}

void AcctInfoItem::setDetailInfo(int index, const QString &iconFilePath, const QString &nickName, const QString &acctNumber)
{
    this->iconFilePath = iconFilePath;
    this->nickNameString = nickName;
    this->acctNoString = acctNumber;
    this->index = index;

    this->nickName->setText(this->nickNameString);
    acctNo->setText(this->acctNoString);
    if(!iconFilePath.isEmpty())
    {
        QPixmap iconPixmap(iconFilePath);
        iconPixmap = getRoundImage(iconPixmap);
        icon->setPixmap(iconPixmap);
    }
    else
    {
        QPixmap iconPixmap(defaultIconFilePath);
        iconPixmap = getRoundImage(iconPixmap);
        icon->setPixmap(iconPixmap);
    }
}


QPixmap AcctInfoItem::getRoundImage(const QPixmap &src)
{
    int radius = 15;
    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    return pixmap;
}

void AcctInfoItem::onDelButtonClicked()
{
    emit removeAcct(index, nickNameString, acctNoString, iconFilePath);
}
