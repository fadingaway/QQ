#include "acctinfodetail.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QPixmap"
#include "QPushButton"
#include "QPainter"
#include "QListWidgetItem"
#include "QMouseEvent"
#include "QDebug"

AcctInfoDetail::AcctInfoDetail(QWidget *parent) :QWidget(parent)
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
    connect(del, &QPushButton::clicked, this, &AcctInfoDetail::onDelButtonClicked);
    del->setHidden(true);

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
    this->setMouseTracking(true);

    QSizePolicy sizepolicy = del->sizePolicy();
    sizepolicy.setRetainSizeWhenHidden(true);
    del->setSizePolicy(sizepolicy);
}

AcctInfoDetail::~AcctInfoDetail()
{

}

void AcctInfoDetail::setDetailInfo(const QString &nickName, const QString &acctNumber, const QString &iconFilePath)
{
    this->iconFilePath = iconFilePath;
    this->nickNameString = nickName;
    this->acctNoString = acctNumber;

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


QPixmap AcctInfoDetail::getRoundImage(const QPixmap &src)
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

void AcctInfoDetail::onDelButtonClicked()
{
    emit removeAcct(nickNameString, acctNoString, iconFilePath);
}

void AcctInfoDetail::onClicked()
{
    emit showAcctInfo(nickNameString, acctNoString, iconFilePath);
}

void AcctInfoDetail::setDelButtonStatus(bool hide)
{
    del->setHidden(hide);
}

bool AcctInfoDetail::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this)
    {
        if(event->type() == QEvent::Enter)
        {
            del->setHidden(true);
        }
        else if(event->type() == QEvent::Leave)
        {
            del->setHidden(false);
        }
        event->accept();
    }
    return QWidget::eventFilter(watched, event);
}

void AcctInfoDetail::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        isMousePressed = true;
}
void AcctInfoDetail::mouseReleaseEvent(QMouseEvent *event)
{
    if(isMousePressed)
        emit onClicked();
    isMousePressed = false;
}
